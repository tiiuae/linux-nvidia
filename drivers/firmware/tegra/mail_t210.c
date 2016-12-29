/*
 * Copyright (c) 2013-2017, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/io.h>
#include <linux/of_address.h>
#include <soc/tegra/doorbell.h>
#include "bpmp.h"

static void __iomem *arb_sema;

/* CPU to BPMP atomic channels */
#define CPU0_OB_CH0		0
#define CPU1_OB_CH0		1
#define CPU2_OB_CH0		2
#define CPU3_OB_CH0		3

/* CPU to BPMP non-atomic channels */
#define CPU0_OB_CH1		4
#define CPU1_OB_CH1		5
#define CPU2_OB_CH1		6
#define CPU3_OB_CH1		7

/* BPMP to CPU channels */
#define CPU0_IB_CH		8
#define CPU1_IB_CH		9
#define CPU2_IB_CH		10
#define CPU3_IB_CH		11

#define CPU_OB_DOORBELL		4

#define TRIGGER_OFFSET		0x000
#define RESULT_OFFSET(id)	(0xc00 + id * 4)
#define TRIGGER_ID_SHIFT	16
#define TRIGGER_CMD_GET		4

#define STA_OFFSET		0
#define SET_OFFSET		4
#define CLR_OFFSET		8

#define PER_CPU_IB_CH(i)	(CPU0_IB_CH + i)

/*
 * How the token bits are interpretted
 *
 * SL_SIGL (b00): slave ch in signalled state
 * SL_QUED (b01): slave ch is in queue
 * MA_FREE (b10): master ch is free
 * MA_ACKD (b11): master ch is acked
 *
 * Ideally, the slave should only set bits while the
 * master do only clear them. But there is an exception -
 * see bpmp_ack_master()
 */
#define CH_MASK(ch)	(0x3 << ((ch) * 2))
#define SL_SIGL(ch)	(0x0 << ((ch) * 2))
#define SL_QUED(ch)	(0x1 << ((ch) * 2))
#define MA_FREE(ch)	(0x2 << ((ch) * 2))
#define MA_ACKD(ch)	(0x3 << ((ch) * 2))

static u32 bpmp_ch_sta(int ch)
{
	return __raw_readl(arb_sema + STA_OFFSET) & CH_MASK(ch);
}

static bool bpmp_master_free(const struct mail_ops *ops, int ch)
{
	return bpmp_ch_sta(ch) == MA_FREE(ch);
}

static bool bpmp_slave_signalled(const struct mail_ops *ops, int ch)
{
	return bpmp_ch_sta(ch) == SL_SIGL(ch);
}

static bool bpmp_master_acked(const struct mail_ops *ops, int ch)
{
	return bpmp_ch_sta(ch) == MA_ACKD(ch);
}

static void bpmp_signal_slave(const struct mail_ops *ops, int ch)
{
	__raw_writel(CH_MASK(ch), arb_sema + CLR_OFFSET);
}

static void bpmp_ack_master(int ch, int flags)
{
	__raw_writel(MA_ACKD(ch), arb_sema + SET_OFFSET);

	if (flags & DO_ACK)
		return;

	/*
	 * We have to violate the bit modification rule while
	 * moving from SL_QUED to MA_FREE (DO_ACK not set) so that
	 * the channel won't be in ACKD state forever.
	 */
	__raw_writel(MA_ACKD(ch) ^ MA_FREE(ch), arb_sema + CLR_OFFSET);
}

/* MA_ACKD to MA_FREE */
static void bpmp_free_master(const struct mail_ops *ops, int ch)
{
	__raw_writel(MA_ACKD(ch) ^ MA_FREE(ch), arb_sema + CLR_OFFSET);
}

static void bpmp_ring_doorbell(int ch)
{
	tegra_ring_doorbell(CPU_OB_DOORBELL);
}

static void bpmp_return_data(const struct mail_ops *ops,
		int ch, int code, void *data, int sz)
{
	struct mb_data *p;
	int flags;

	if (sz > MSG_DATA_MIN_SZ) {
		WARN_ON(1);
		return;
	}

	p = channel_area[ch].ob;
	p->code = code;
	memcpy(p->data, data, sz);

	flags = channel_area[ch].ib->flags;
	bpmp_ack_master(ch, flags);
	if (flags & RING_DOORBELL)
		bpmp_ring_doorbell(ch);
}

static int bpmp_thread_ch_index(int ch)
{
	if (ch < CPU0_OB_CH1 || ch > CPU3_OB_CH1)
		return -1;
	return ch - CPU0_OB_CH1;
}

static int bpmp_thread_ch(int idx)
{
	return CPU0_OB_CH1 + idx;
}

static int bpmp_ob_channel(void)
{
	return smp_processor_id() + CPU0_OB_CH0;
}

static void bpmp_doorbell_handler(void *data)
{
	int ch = (long)data;

	bpmp_handle_irq(ch);
}

static int bpmp_init_irq(void)
{
	long ch;
	int r;
	int i;

	for (i = 0; i < 4; i++) {
		ch = PER_CPU_IB_CH(i);
		r = tegra_register_doorbell_handler(i, bpmp_doorbell_handler,
						   (void *)ch);
		if (r)
			return r;
	}

	return 0;
}

/* Channel area is setup by BPMP before signalling handshake */
static u32 bpmp_channel_area(void __iomem *atomics, int ch)
{
	u32 a;

	writel(ch << TRIGGER_ID_SHIFT | TRIGGER_CMD_GET,
			atomics + TRIGGER_OFFSET);
	a = readl(atomics + RESULT_OFFSET(ch));

	return a;
}

static int bpmp_connect(const struct mail_ops *ops, struct device_node *of_node)
{
	uint32_t channel_hwaddr[NR_CHANNELS];
	void __iomem *atomics;
	void *p;
	int i;

	atomics = of_iomap(of_node, 0);
	if (!atomics)
		return -ENODEV;

	arb_sema = of_iomap(of_node, 1);
	if (!arb_sema)
		return -ENODEV;

	/* handshake */
	if (!readl(arb_sema + STA_OFFSET))
		return -ENODEV;

	for (i = 0; i < NR_CHANNELS; i++) {
		channel_hwaddr[i] = bpmp_channel_area(atomics, i);
		if (!channel_hwaddr[i])
			return -EFAULT;
	}

	for (i = 0; i < NR_CHANNELS; i++) {
		p = ioremap(channel_hwaddr[i], 0x80);

		channel_area[i].ib = p;
		channel_area[i].ob = p;
	}

	return 0;
}

const struct mail_ops t210_mail_ops = {
	.init_irq = bpmp_init_irq,
	.connect = bpmp_connect,
	.ob_channel = bpmp_ob_channel,
	.thread_ch = bpmp_thread_ch,
	.thread_ch_index = bpmp_thread_ch_index,
	.master_free = bpmp_master_free,
	.free_master = bpmp_free_master,
	.master_acked = bpmp_master_acked,
	.signal_slave = bpmp_signal_slave,
	.ring_doorbell = bpmp_ring_doorbell,
	.slave_signalled = bpmp_slave_signalled,
	.return_data = bpmp_return_data
};
