/*
 * Copyright (c) 2021-2022, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#include "t234_hwpm_ip_mss_gpu_hub.h"

#include <tegra_hwpm.h>
#include <hal/t234/t234_hwpm_regops_allowlist.h>
#include <hal/t234/hw/t234_addr_map_soc_hwpm.h>

hwpm_ip_perfmon t234_mss_gpu_hub_perfmon_static_array[
	T234_HWPM_IP_MSS_GPU_HUB_NUM_PERFMON_PER_INST *
	T234_HWPM_IP_MSS_GPU_HUB_NUM_INSTANCES] = {
	{
		.is_hwpm_element = true,
		.hw_inst_mask = BIT(0),
		.dt_mmio = NULL,
		.name = "perfmon_mssnvlhsh0",
		.ip_ops = {
			.ip_base_address = 0ULL,
			.ip_index = 0U,
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
		},
		.start_abs_pa = addr_map_rpg_pm_mssnvl_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mssnvl_limit_r(),
		.start_pa = 0,
		.end_pa = 0,
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = t234_perfmon_alist,
		.alist_size = ARRAY_SIZE(t234_perfmon_alist),
		.fake_registers = NULL,
	},
};

hwpm_ip_perfmux t234_mss_gpu_hub_perfmux_static_array[
	T234_HWPM_IP_MSS_GPU_HUB_NUM_PERFMUX_PER_INST *
	T234_HWPM_IP_MSS_GPU_HUB_NUM_INSTANCES] = {
	{
		.is_hwpm_element = false,
		.hw_inst_mask = BIT(0),
		.dt_mmio = NULL,
		.name = {'\0'},
		.ip_ops = {
			.ip_base_address = 0ULL,
			.ip_index = 0U,
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
		},
		.start_abs_pa = addr_map_mss_nvlink_8_base_r(),
		.end_abs_pa = addr_map_mss_nvlink_8_limit_r(),
		.start_pa = 0,
		.end_pa = 0,
		.base_pa = 0ULL,
		.alist = t234_mss_nvlink_alist,
		.alist_size = ARRAY_SIZE(t234_mss_nvlink_alist),
		.fake_registers = NULL,
	},
	{
		.is_hwpm_element = false,
		.hw_inst_mask = BIT(0),
		.dt_mmio = NULL,
		.name = {'\0'},
		.ip_ops = {
			.ip_base_address = 0ULL,
			.ip_index = 0U,
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
		},
		.start_abs_pa = addr_map_mss_nvlink_1_base_r(),
		.end_abs_pa = addr_map_mss_nvlink_1_limit_r(),
		.start_pa = 0,
		.end_pa = 0,
		.base_pa = 0ULL,
		.alist = t234_mss_nvlink_alist,
		.alist_size = ARRAY_SIZE(t234_mss_nvlink_alist),
		.fake_registers = NULL,
	},
	{
		.is_hwpm_element = false,
		.hw_inst_mask = BIT(0),
		.dt_mmio = NULL,
		.name = {'\0'},
		.ip_ops = {
			.ip_base_address = 0ULL,
			.ip_index = 0U,
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
		},
		.start_abs_pa = addr_map_mss_nvlink_2_base_r(),
		.end_abs_pa = addr_map_mss_nvlink_2_limit_r(),
		.start_pa = 0,
		.end_pa = 0,
		.base_pa = 0ULL,
		.alist = t234_mss_nvlink_alist,
		.alist_size = ARRAY_SIZE(t234_mss_nvlink_alist),
		.fake_registers = NULL,
	},
	{
		.is_hwpm_element = false,
		.hw_inst_mask = BIT(0),
		.dt_mmio = NULL,
		.name = {'\0'},
		.ip_ops = {
			.ip_base_address = 0ULL,
			.ip_index = 0U,
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
		},
		.start_abs_pa = addr_map_mss_nvlink_3_base_r(),
		.end_abs_pa = addr_map_mss_nvlink_3_limit_r(),
		.start_pa = 0,
		.end_pa = 0,
		.base_pa = 0ULL,
		.alist = t234_mss_nvlink_alist,
		.alist_size = ARRAY_SIZE(t234_mss_nvlink_alist),
		.fake_registers = NULL,
	},
	{
		.is_hwpm_element = false,
		.hw_inst_mask = BIT(0),
		.dt_mmio = NULL,
		.name = {'\0'},
		.ip_ops = {
			.ip_base_address = 0ULL,
			.ip_index = 0U,
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
		},
		.start_abs_pa = addr_map_mss_nvlink_4_base_r(),
		.end_abs_pa = addr_map_mss_nvlink_4_limit_r(),
		.start_pa = 0,
		.end_pa = 0,
		.base_pa = 0ULL,
		.alist = t234_mss_nvlink_alist,
		.alist_size = ARRAY_SIZE(t234_mss_nvlink_alist),
		.fake_registers = NULL,
	},
	{
		.is_hwpm_element = false,
		.hw_inst_mask = BIT(0),
		.dt_mmio = NULL,
		.name = {'\0'},
		.ip_ops = {
			.ip_base_address = 0ULL,
			.ip_index = 0U,
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
		},
		.start_abs_pa = addr_map_mss_nvlink_5_base_r(),
		.end_abs_pa = addr_map_mss_nvlink_5_limit_r(),
		.start_pa = 0,
		.end_pa = 0,
		.base_pa = 0ULL,
		.alist = t234_mss_nvlink_alist,
		.alist_size = ARRAY_SIZE(t234_mss_nvlink_alist),
		.fake_registers = NULL,
	},
	{
		.is_hwpm_element = false,
		.hw_inst_mask = BIT(0),
		.dt_mmio = NULL,
		.name = {'\0'},
		.ip_ops = {
			.ip_base_address = 0ULL,
			.ip_index = 0U,
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
		},
		.start_abs_pa = addr_map_mss_nvlink_6_base_r(),
		.end_abs_pa = addr_map_mss_nvlink_6_limit_r(),
		.start_pa = 0,
		.end_pa = 0,
		.base_pa = 0ULL,
		.alist = t234_mss_nvlink_alist,
		.alist_size = ARRAY_SIZE(t234_mss_nvlink_alist),
		.fake_registers = NULL,
	},
	{
		.is_hwpm_element = false,
		.hw_inst_mask = BIT(0),
		.dt_mmio = NULL,
		.name = {'\0'},
		.ip_ops = {
			.ip_base_address = 0ULL,
			.ip_index = 0U,
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
		},
		.start_abs_pa = addr_map_mss_nvlink_7_base_r(),
		.end_abs_pa = addr_map_mss_nvlink_7_limit_r(),
		.start_pa = 0,
		.end_pa = 0,
		.base_pa = 0ULL,
		.alist = t234_mss_nvlink_alist,
		.alist_size = ARRAY_SIZE(t234_mss_nvlink_alist),
		.fake_registers = NULL,
	},
};

struct hwpm_ip t234_hwpm_ip_mss_gpu_hub = {
	.num_instances = T234_HWPM_IP_MSS_GPU_HUB_NUM_INSTANCES,
	.num_perfmon_per_inst = T234_HWPM_IP_MSS_GPU_HUB_NUM_PERFMON_PER_INST,
	.num_perfmux_per_inst = T234_HWPM_IP_MSS_GPU_HUB_NUM_PERFMUX_PER_INST,

	.perfmon_range_start = addr_map_rpg_pm_mssnvl_base_r(),
	.perfmon_range_end = addr_map_rpg_pm_mssnvl_limit_r(),
	.inst_perfmon_stride = addr_map_rpg_pm_mssnvl_limit_r() -
			addr_map_rpg_pm_mssnvl_base_r() + 1ULL,
	.num_perfmon_slots = 0U,
	.ip_perfmon = NULL,

	.perfmux_range_start = addr_map_mss_nvlink_8_base_r(),
	.perfmux_range_end = addr_map_mss_nvlink_7_limit_r(),
	.inst_perfmux_stride = addr_map_mss_nvlink_8_limit_r() -
			addr_map_mss_nvlink_8_base_r() + 1ULL,
	.num_perfmux_slots = 0U,
	.ip_perfmux = NULL,

	.override_enable = false,
	.fs_mask = 0U,
	.perfmon_static_array = t234_mss_gpu_hub_perfmon_static_array,
	.perfmux_static_array = t234_mss_gpu_hub_perfmux_static_array,
	.reserved = false,
};