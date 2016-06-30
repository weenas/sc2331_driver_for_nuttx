/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * Authors	:
 * Keguang Zhang <keguang.zhang@spreadtrum.com>
 * Jingxiang Li <Jingxiang.li@spreadtrum.com>
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

#include "sc2331.h"
#include "sdio_ba.h"
#include "nuttx.h"
//#include "msg.h"
//#include "txrx.h"

#define SPRDWL_TX_MSG_NUM 256
#define SPRDWL_TX_DATA_STOPE_QUEU_NUM (SPRDWL_TX_MSG_NUM - 4)
#define SPRDWL_TX_DATA_START_QUEU_NUM (SPRDWL_TX_DATA_STOPE_QUEU_NUM - 4)
#define SPRDWL_RX_MSG_NUM 10
#define HW_TX_SIZE              (13 * 1024)
#define HW_RX_SIZE              (12288)
#define PKT_AGGR_NUM            (14)
#define ALIGN_4BYTE(a)		(((a) + 3) & ~3)

static struct sprdwl_sdio *sprdwl_sdev;

static void sprdwl_clean_txinfo(struct sprdwl_tx_buf *txbuf)
{
	unsigned int *num;

	txbuf->curpos = 4;
	num = (int *)txbuf->base;
	*num = 0;
}

static int sprdwl_rx_process(struct sprdwl_priv *priv,
			     unsigned char *buf, unsigned int max_len)
{
	unsigned short len;
	unsigned int p = 0;
	unsigned char *pdata = 0;

	if (!buf || (max_len < 8)) {
		return 0;
	}

	buf += 8;
	max_len -= 8;
	while (p < max_len) {
		pdata = buf + p;
		if (pdata[0] == 0xff)
			return 0;
		switch (SPRDWL_HEAD_GET_TYPE(pdata)) {
		case SPRDWL_TYPE_DATA:
			len = sprdwl_rx_data_process(priv, pdata);
			break;
		case SPRDWL_TYPE_CMD:
			len = sprdwl_rx_rsp_process(priv, pdata);
			break;
		case SPRDWL_TYPE_EVENT:
			len = sprdwl_rx_event_process(priv, pdata);
			break;
		default:
			len = 0;
			break;
		}
		if (len) {
			p += ALIGN_4BYTE(len);
		} else {
			return 0;
		}
	}

	return 0;
}

static int sprdwl_hw_init(struct hw_info *hw)
{
	memset(hw, 0, sizeof(struct hw_info));

	hw->sdio_tx_chn.num = 3;
	hw->sdio_tx_chn.chn[0] = 0;
	hw->sdio_tx_chn.chn[1] = 1;
	hw->sdio_tx_chn.chn[2] = 2;
	hw->sdio_tx_chn.bit_map = 0x0007;
	hw->sdio_tx_chn.timeout_time = 600;
	hw->sdio_tx_chn.timeout_flag = 0;

	hw->sdio_rx_chn.num = 6;
	hw->sdio_rx_chn.chn[0] = 8;
	hw->sdio_rx_chn.chn[1] = 9;
	hw->sdio_rx_chn.chn[2] = 14;
	hw->sdio_rx_chn.chn[3] = 11;
	hw->sdio_rx_chn.chn[4] = 15;
	hw->sdio_rx_chn.chn[5] = 13;
	hw->sdio_rx_chn.bit_map = 0xeb00;
	hw->sdio_rx_chn.gpio_high = 0;
	//hw->sdio_rx_chn.timeout_time = 3000;
	//hw->sdio_rx_chn.timeout_flag = false;

	hw->wakeup = 0;
	return 0;
}

void sprdwl_rx_chn_isr(int chn)
{

}

static int check_valid_chn(int flag, unsigned short status,
			   struct sdio_chn *chn_info)
{
	int i, index = -1;

	if (flag == 1)
		status = (status & chn_info->bit_map);
	else
		status = ((status & chn_info->bit_map) ^ (chn_info->bit_map));
	if (!status)
		return -1;
	for (i = 0; i < chn_info->num; i++) {
		if (status & (0x1 << chn_info->chn[i])) {
			index = chn_info->chn[i];
			break;
		}
	}
	return index;
}

static int sprdwl_send_packet(struct sprdwl_sdio *sdev,
			      struct sprdwl_tx_buf *txbuf, int retry)
{
	unsigned short status;
	int ret, index;
	struct sdio_chn *tx_chn;

	if (txbuf->curpos == 4)
		return 0;
	tx_chn = &sdev->hw.sdio_tx_chn;
get_status:
	ret = set_marlin_wakeup(0, 1);
	if (ret) {
		if (ret == -3) {
			usleep_range(2000, 3000);
			return -1;
		}
		if (ret != -2) {
			msleep(50);
			return -1;
		}
	}
	ret = sdio_chn_status(tx_chn->bit_map, &status);
	if (ret) {
		if (retry) {
			retry--;
			usleep_range(200, 400);
			goto get_status;
		}
		return -1;
	}
	index = check_valid_chn(0, status, tx_chn);
	if (index < 0) {
		if (retry) {
			retry--;
			usleep_range(40, 50);
			goto get_status;
		}
		return -1;
	}
resend:
	ret = sdio_dev_write(index, txbuf->base,
			     (txbuf->curpos + 1023) & (~0x3FF));
	if (ret) {
		if (retry) {
			retry--;
			goto resend;
		}
		return -1;
	}
	sprdwl_clean_txinfo(txbuf);

	return 0;
}

static void sprdwl_sdio_force_exit(void)
{
	sprdwl_sdev->exit = 1;
}

static int sprdwl_sdio_is_exit(void)
{
	return sprdwl_sdev->exit;
}

static struct sprdwl_if_ops sprdwl_sdio_ops = {
//      .tx = sprdwl_sdio_txmsg,
	.force_exit = sprdwl_sdio_force_exit,
	.is_exit = sprdwl_sdio_is_exit,
};

int sprdwl_intf_init(struct sprdwl_priv *priv)
{
	int ret = 0;

	priv->if_ops = &sprdwl_sdio_ops;
	//sprdwl_hw_init(&sdev->hw);
	invalid_recv_flush(8);
	invalid_recv_flush(9);
	/* flush marlin log */
	invalid_recv_flush(14);
	sdiodev_readchn_init(8, (void *)sprdwl_rx_chn_isr, 1);
	sdiodev_readchn_init(9, (void *)sprdwl_rx_chn_isr, 1);

	return ret;
}
