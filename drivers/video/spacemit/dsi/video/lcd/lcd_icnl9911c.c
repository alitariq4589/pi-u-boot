// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2023 Spacemit Co., Ltd.
 *
 */

#include <linux/kernel.h>
#include "../../include/spacemit_dsi_common.h"
#include "../../include/spacemit_video_tx.h"
#include <linux/delay.h>

#define UNLOCK_DELAY 0

struct spacemit_mode_modeinfo icnl9911c_spacemit_modelist[] = {
	{
		.name = "720x1600-60",
		.refresh = 60,
		.xres = 720,
		.yres = 1600,
		.real_xres = 720,
		.real_yres = 1600,
		.left_margin = 48,
		.right_margin = 48,
		.hsync_len = 4,
		.upper_margin = 32,
		.lower_margin = 150,
		.vsync_len = 4,
		.hsync_invert = 0,
		.vsync_invert = 0,
		.invert_pixclock = 0,
		.pixclock_freq = 87*1000,
		.pix_fmt_out = OUTFMT_RGB888,
		.width = 72,
		.height = 126,
	},
};

struct spacemit_mipi_info icnl9911c_mipi_info = {
	.height = 1600,
	.width = 720,
	.hfp = 48,/* unit: pixel */
	.hbp = 48,
	.hsync = 4,
	.vfp = 150, /*unit: line*/
	.vbp = 32,
	.vsync = 4,
	.fps = 60,

	.work_mode = SPACEMIT_DSI_MODE_VIDEO, /*command_mode, video_mode*/
	.rgb_mode = DSI_INPUT_DATA_RGB_MODE_888,
	.lane_number = 4,
	.phy_bit_clock = 614400000,
	.phy_esc_clock = 51200000,
	.split_enable = 0,
	.eotp_enable = 0,

	.burst_mode = DSI_BURST_MODE_BURST,
};

static struct spacemit_dsi_cmd_desc icnl9911c_set_id_cmds[] = {
	{SPACEMIT_DSI_SET_MAX_PKT_SIZE, SPACEMIT_DSI_LP_MODE, UNLOCK_DELAY, 1, {0x01}},
};

static struct spacemit_dsi_cmd_desc icnl9911c_read_id_cmds[] = {
	{SPACEMIT_DSI_GENERIC_READ1, SPACEMIT_DSI_LP_MODE, UNLOCK_DELAY, 1, {0x04}},
};

static struct spacemit_dsi_cmd_desc icnl9911c_set_power_cmds[] = {
	{SPACEMIT_DSI_SET_MAX_PKT_SIZE, SPACEMIT_DSI_HS_MODE, UNLOCK_DELAY, 1, {0x1}},
};

static struct spacemit_dsi_cmd_desc icnl9911c_read_power_cmds[] = {
	{SPACEMIT_DSI_GENERIC_READ1, SPACEMIT_DSI_HS_MODE, UNLOCK_DELAY, 1, {0xA}},
};

static struct spacemit_dsi_cmd_desc icnl9911c_init_cmds[] = {
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,   3, {0xF0, 0x5A, 0x59}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,   3, {0xF1, 0xA5, 0xA6}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  33, {0xB0, 0x83, 0x82, 0x86, 0x87, 0x06, 0x07, 0x04, 0x05, 0x33, 0x33, 0x33, 0x33, 0x20, 0x00, 0x00, 0x77, 0x00, 0x00, 0x3F, 0x05, 0x04, 0x03, 0x02, 0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  30, {0xB1, 0x13, 0x91, 0x8E, 0x81, 0x20, 0x00, 0x00, 0x77, 0x00, 0x00, 0x04, 0x08, 0x54, 0x00, 0x00, 0x00, 0x44, 0x40, 0x02, 0x01, 0x40, 0x02, 0x01, 0x40, 0x02, 0x01, 0x40, 0x02, 0x01}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  18, {0xB2, 0x54, 0xC4, 0x82, 0x05, 0x40, 0x02, 0x01, 0x40, 0x02, 0x01, 0x05, 0x05, 0x54, 0x0C, 0x0C, 0x0D, 0x0B}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  32, {0xB3, 0x12, 0x00, 0x00, 0x00, 0x00, 0x26, 0x26, 0x91, 0x91, 0x91, 0x91, 0x3C, 0x26, 0x00, 0x18, 0x01, 0x02, 0x08, 0x20, 0x30, 0x08, 0x09, 0x44, 0x20, 0x40, 0x20, 0x40, 0x08, 0x09, 0x22, 0x33}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  29, {0xB4, 0x03, 0x00, 0x00, 0x06, 0x1E, 0x1F, 0x0C, 0x0E, 0x10, 0x12, 0x14, 0x16, 0x04, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  29, {0xB5, 0x03, 0x00, 0x00, 0x07, 0x1E, 0x1F, 0x0D, 0x0F, 0x11, 0x13, 0x15, 0x17, 0x05, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  25, {0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,   3, {0xBA, 0x6B, 0x6B}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  14, {0xBB, 0x01, 0x05, 0x09, 0x11, 0x0D, 0x19, 0x1D, 0x55, 0x25, 0x69, 0x00, 0x21, 0x25}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  15, {0xBC, 0x00, 0x00, 0x00, 0x00, 0x02, 0x20, 0xFF, 0x00, 0x03, 0x33, 0x01, 0x73, 0x33, 0x02}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  11, {0xBD, 0xE9, 0x02, 0x4F, 0xCF, 0x72, 0xA4, 0x08, 0x44, 0xAE, 0x15}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  13, {0xBE, 0x7D, 0x7D, 0x5A, 0x46, 0x0C, 0x77, 0x43, 0x07, 0x0E, 0x0E, 0x00, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,   9, {0xBF, 0x07, 0x25, 0x07, 0x25, 0x7F, 0x00, 0x11, 0x04}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  13, {0xC0, 0x10, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  20, {0xC1, 0xC0, 0x20, 0x20, 0x96, 0x04, 0x32, 0x32, 0x04, 0x2A, 0x40, 0x36, 0x00, 0x07, 0xCF, 0xFF, 0xFF, 0xC0, 0x00, 0xC0}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,   2, {0xC2, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  16, {0xC2, 0xCC, 0x01, 0x10, 0x00, 0x01, 0x30, 0x02, 0x21, 0x43, 0x00, 0x01, 0x30, 0x02, 0x21, 0x43}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  13, {0xC3, 0x06, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x81, 0x01, 0x00, 0x00, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  13, {0xC4, 0x84, 0x03, 0x2B, 0x41, 0x00, 0x3C, 0x00, 0x03, 0x03, 0x3E, 0x00, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  12, {0xC5, 0x03, 0x1C, 0xC0, 0xC0, 0x40, 0x10, 0x42, 0x44, 0x0F, 0x0A, 0x14}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  11, {0xC6, 0x87, 0xA0, 0x2A, 0x29, 0x29, 0x00, 0x64, 0x37, 0x08, 0x04}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  23, {0xC7, 0xF7, 0xD3, 0xBA, 0xA5, 0x80, 0x63, 0x36, 0x8B, 0x56, 0x2A, 0xFF, 0xCE, 0x23, 0xF4, 0xD3, 0xA4, 0x86, 0x5A, 0x1A, 0x7F, 0xE4, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  23, {0xC8, 0xF7, 0xD3, 0xBA, 0xA5, 0x80, 0x63, 0x36, 0x8B, 0x56, 0x2A, 0xFF, 0xCE, 0x23, 0xF4, 0xD3, 0xA4, 0x86, 0x5A, 0x1A, 0x7F, 0xE4, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,   9, {0xD0, 0x80, 0x0D, 0xFF, 0x0F, 0x61, 0x0B, 0x08, 0x0C}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,  15, {0xD2, 0x42, 0x0C, 0x30, 0x01, 0x80, 0x26, 0x04, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,   3, {0xF1, 0x5A, 0x59}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,   3, {0xF0, 0xA5, 0xA6}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 0,   2, {0x35, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE, 150, 2, {0x11, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE,  50, 2, {0x29, 0x00}},
	{SPACEMIT_DSI_DCS_LWRITE, SPACEMIT_DSI_LP_MODE,   5, 2, {0x26, 0x00}},
};

static struct spacemit_dsi_cmd_desc icnl9911c_sleep_out_cmds[] = {
	{SPACEMIT_DSI_DCS_SWRITE,SPACEMIT_DSI_LP_MODE,150,1,{0x11}},
	{SPACEMIT_DSI_DCS_SWRITE,SPACEMIT_DSI_LP_MODE,50,1,{0x29}},
};

static struct spacemit_dsi_cmd_desc icnl9911c_sleep_in_cmds[] = {
	{SPACEMIT_DSI_DCS_SWRITE,SPACEMIT_DSI_LP_MODE,50,1,{0x28}},
	{SPACEMIT_DSI_DCS_SWRITE,SPACEMIT_DSI_LP_MODE,150,1,{0x10}},
};


struct lcd_mipi_panel_info lcd_icnl9911c = {
	.lcd_name = "icnl9911c",
	.lcd_id = 0x7202,
	.panel_id0 = 0x99,
	.power_value = 0x9c,
	.panel_type = LCD_MIPI,
	.width_mm = 72,
	.height_mm = 126,
	.dft_pwm_bl = 128,
	.set_id_cmds_num = ARRAY_SIZE(icnl9911c_set_id_cmds),
	.read_id_cmds_num = ARRAY_SIZE(icnl9911c_read_id_cmds),
	.init_cmds_num = ARRAY_SIZE(icnl9911c_init_cmds),
	.set_power_cmds_num = ARRAY_SIZE(icnl9911c_set_power_cmds),
	.read_power_cmds_num = ARRAY_SIZE(icnl9911c_read_power_cmds),
	.sleep_out_cmds_num = ARRAY_SIZE(icnl9911c_sleep_out_cmds),
	.sleep_in_cmds_num = ARRAY_SIZE(icnl9911c_sleep_in_cmds),
	//.drm_modeinfo = icnl9911c_modelist,
	.spacemit_modeinfo = icnl9911c_spacemit_modelist,
	.mipi_info = &icnl9911c_mipi_info,
	.set_id_cmds = icnl9911c_set_id_cmds,
	.read_id_cmds = icnl9911c_read_id_cmds,
	.set_power_cmds = icnl9911c_set_power_cmds,
	.read_power_cmds = icnl9911c_read_power_cmds,
	.init_cmds = icnl9911c_init_cmds,
	.sleep_out_cmds = icnl9911c_sleep_out_cmds,
	.sleep_in_cmds = icnl9911c_sleep_in_cmds,
	.bitclk_sel = 3,
	.bitclk_div = 1,
	.pxclk_sel = 2,
	.pxclk_div = 6,
};

int lcd_icnl9911c_init(void)
{
	int ret;

	ret = lcd_mipi_register_panel(&lcd_icnl9911c);
	return ret;
}
