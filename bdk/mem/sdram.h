/*
 * Copyright (c) 2018 naehrwert
 * Copyright (c) 2020-2022 CTCaer
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

#ifndef _SDRAM_H_
#define _SDRAM_H_

#include <mem/emc.h>

/*
 * Tegra X1/X1+ EMC/DRAM Bandwidth Chart:
 *
 * Note: BWbits T210    = Hz x ddr x bus width x channels = Hz x 2 x 32 x 2.
 *       BWbits T210B01 = Hz x ddr x bus width x channels = Hz x 2 x 64 x 2.
 *       Both assume that both sub-partitions are used and thus reaching max
 *       bandwidth per channel. (T210: 2x16-bit, T210B01: 2x32-bit).
 *       Retail Mariko use one sub-partition, in order to meet Erista perf.
 *
 *              T210   T210B01
 *   40.8 MHz:  0.61   1.22 GiB/s
 *   68.0 MHz:  1.01   2.02 GiB/s
 *  102.0 MHz:  1.52   3.04 GiB/s
 *  204.0 MHz:  3.04   6.08 GiB/s <-- Tegra X1/X1+ Init/SC7 Frequency
 *  408.0 MHz:  6.08  12.16 GiB/s
 *  665.6 MHz:  9.92  19.84 GiB/s
 *  800.0 MHz: 11.92  23.84 GiB/s <-- Tegra X1/X1+ Nvidia OS Boot Frequency
 * 1065.6 MHz: 15.89  31.78 GiB/s
 * 1331.2 MHz: 19.84  39.68 GiB/s
 * 1600.0 MHz: 23.84  47.68 GiB/s <-- Tegra X1/X1+ HOS Max Frequency
 * 1862.4 MHz: 27.75  55.50 GiB/s <-- Tegra X1  Official Max Frequency
 * 2131.2 MHz: 31.76  63.52 GiB/s <-- Tegra X1+ Official Max Frequency
 *
 */

enum sdram_ids_erista
{
	// LPDDR4 3200Mbps.
	LPDDR4_ICOSA_4GB_SAMSUNG_K4F6E304HB_MGCH        = 0,
	LPDDR4_ICOSA_4GB_HYNIX_H9HCNNNBPUMLHR_NLE       = 1,
	LPDDR4_ICOSA_4GB_MICRON_MT53B512M32D2NP_062_WT  = 2, // WT:C.

	LPDDR4_ICOSA_6GB_SAMSUNG_K4FHE3D4HM_MGCH        = 4,
};

enum sdram_ids_mariko
{
	// LPDDR4X 4266Mbps.
	LPDDR4X_HOAG_4GB_HYNIX_H9HCNNNBKMMLXR_NEE       = 3, // Replaced from Copper. Die-M. (1y-01).
	LPDDR4X_IOWA_4GB_HYNIX_H9HCNNNBKMMLXR_NEE       = 5, // Replaced from Copper. Die-M. (1y-01).
	LPDDR4X_AULA_4GB_HYNIX_H9HCNNNBKMMLXR_NEE       = 6, // Replaced from Copper. Die-M. (1y-01).

	// LPDDR4X 3733Mbps.
	LPDDR4X_IOWA_4GB_SAMSUNG_K4U6E3S4AM_MGCJ        = 8,  // Die-M.
	LPDDR4X_IOWA_8GB_SAMSUNG_K4UBE3D4AM_MGCJ        = 9,  // Die-M.
	LPDDR4X_IOWA_4GB_HYNIX_H9HCNNNBKMMLHR_NME       = 10, // Die-M.
	LPDDR4X_IOWA_4GB_MICRON_MT53E512M32D2NP_046_WTE = 11, // 4266Mbps. Die-E.

	LPDDR4X_HOAG_4GB_SAMSUNG_K4U6E3S4AM_MGCJ        = 12, // Die-M.
	LPDDR4X_HOAG_8GB_SAMSUNG_K4UBE3D4AM_MGCJ        = 13, // Die-M.
	LPDDR4X_HOAG_4GB_HYNIX_H9HCNNNBKMMLHR_NME       = 14, // Die-M.
	LPDDR4X_HOAG_4GB_MICRON_MT53E512M32D2NP_046_WTE = 15, // 4266Mbps. Die-E.

	// LPDDR4X 4266Mbps.
	LPDDR4X_IOWA_4GB_SAMSUNG_K4U6E3S4AA_MGCL        = 17, // Die-A. (1y-X03).
	LPDDR4X_IOWA_8GB_SAMSUNG_K4UBE3D4AA_MGCL        = 18, // Die-A. (1y-X03).
	LPDDR4X_HOAG_4GB_SAMSUNG_K4U6E3S4AA_MGCL        = 19, // Die-A. (1y-X03).

	LPDDR4X_IOWA_4GB_SAMSUNG_1Z                     = 20, // 1z nm. 40% lower power usage. (1z-01).
	LPDDR4X_HOAG_4GB_SAMSUNG_1Z                     = 21, // 1z nm. 40% lower power usage. (1z-01).
	LPDDR4X_AULA_4GB_SAMSUNG_1Z                     = 22, // 1z nm. 40% lower power usage. (1z-01).

	LPDDR4X_HOAG_8GB_SAMSUNG_K4UBE3D4AA_MGCL        = 23, // Die-A. (1y-X03).
	LPDDR4X_AULA_4GB_SAMSUNG_K4U6E3S4AA_MGCL        = 24, // Die-A. (1y-X03).

	LPDDR4X_IOWA_4GB_MICRON_MT53E512M32D2NP_046_WTF = 25, // 4266Mbps. Die-F. D9XRR. 10nm-class (1y-01).
	LPDDR4X_HOAG_4GB_MICRON_MT53E512M32D2NP_046_WTF = 26, // 4266Mbps. Die-F. D9XRR. 10nm-class (1y-01).
	LPDDR4X_AULA_4GB_MICRON_MT53E512M32D2NP_046_WTF = 27, // 4266Mbps. Die-F. D9XRR. 10nm-class (1y-01).

	LPDDR4X_AULA_8GB_SAMSUNG_K4UBE3D4AA_MGCL        = 28, // Die-A.

	LPDDR4X_UNK0_4GB_HYNIX_1A                       = 29, // 1a nm. 61% lower power usage. (1a-01).
	LPDDR4X_UNK1_4GB_HYNIX_1A                       = 30, // 1a nm. 61% lower power usage. (1a-01).
	LPDDR4X_UNK2_4GB_HYNIX_1A                       = 31, // 1a nm. 61% lower power usage. (1a-01).

	LPDDR4X_UNK0_4GB_MICRON_1A                      = 32, // 1a nm. 61% lower power usage. (1a-01).
	LPDDR4X_UNK1_4GB_MICRON_1A                      = 33, // 1a nm. 61% lower power usage. (1a-01).
	LPDDR4X_UNK2_4GB_MICRON_1A                      = 34, // 1a nm. 61% lower power usage. (1a-01).
};

enum sdram_codes_mariko
{
	LPDDR4X_NO_PATCH                           = 0,
	LPDDR4X_UNUSED                             = 0,

	// LPDDR4X_4GB_SAMSUNG_K4U6E3S4AM_MGCJ          DRAM IDs: 08, 12.
	// LPDDR4X_4GB_HYNIX_H9HCNNNBKMMLHR_NME         DRAM IDs: 10, 14.

	LPDDR4X_8GB_SAMSUNG_K4UBE3D4AM_MGCJ        = 1, // DRAM IDs: 09, 13.
	LPDDR4X_4GB_MICRON_MT53E512M32D2NP_046_WTE = 2, // DRAM IDs: 11, 15.
	LPDDR4X_4GB_SAMSUNG_K4U6E3S4AA_MGCL        = 3, // DRAM IDs: 17, 19, 24.
	LPDDR4X_8GB_SAMSUNG_K4UBE3D4AA_MGCL        = 4, // DRAM IDs: 18, 23, 28.
	LPDDR4X_4GB_SAMSUNG_1Z                     = 5, // DRAM IDs: 20, 21, 22.
	LPDDR4X_4GB_MICRON_MT53E512M32D2NP_046_WTF = 6, // DRAM IDs: 25, 26, 27.
	LPDDR4X_4GB_HYNIX_H9HCNNNBKMMLXR_NEE       = 7, // DRAM IDs: 03, 05, 06.

	LPDDR4X_4GB_HYNIX_1A                       = 8, // DRAM IDs: 29, 30, 31.
	LPDDR4X_4GB_MICRON_1A                      = 9, // DRAM IDs: 32, 33, 34.
};

void sdram_init();
void *sdram_get_params_patched();
void *sdram_get_params_t210b01();
void sdram_lp0_save_params(const void *params);
emc_mr_data_t sdram_read_mrx(emc_mr_t mrx);

#endif
