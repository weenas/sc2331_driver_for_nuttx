/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * Authors	:
 * Eason Xiang <eason@weenas.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __NUTTX_H__
#define __NUTTX_H__

#include "sc2331.h"

static inline int set_marlin_wakeup(unsigned int chn, unsigned int user_id)
{
	return 0;
}

static inline void usleep_range(int a, int b)
{
	return;
}

static inline int sdio_chn_status(unsigned short chn, unsigned short *status)
{
	return 0;
}

static inline int sdio_dev_write(unsigned int chn, void *data_buf, unsigned int count)
{
	return 0;
}

static inline void invalid_recv_flush(unsigned int chn)
{
	return;
}

static inline int sdiodev_readchn_init(int chn, void *callback, unsigned char with_para)
{
	return 0;
}

static inline int sdio_dev_read(unsigned int chn, void *read_buf, unsigned int *count)
{
	return 0;
}

static inline unsigned char get_sdiohal_status(void)
{
	return 0;
}

int sdiodev_readchn_uninit(unsigned int chn);
void mdbg_at_cmd_read(void);
void mdbg_loopcheck_read(void);
void mdbg_sdio_read(void);

#endif

