/*
 * Copyright (C) 2016 Spreadtrum Communications Inc.
 *
 * Authors:<eason@weenas.com>
 * Owner:
 *      Eason Xiang
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

#include <string.h>
#include "cmdevt.h"

#define  CMD_WAIT_TIMEOUT  (3000)

static struct sprdwl_cmd_info cmd;

typedef struct {
	unsigned short id;
	char *cmd;
} cmd_t;

int wlan_cmd_send_recv(unsigned char vif_id, unsigned char *pData, int len,
		       int subtype, int timeout)
{
	return 0;
}

int sprdwl_cmd_init(void)
{
	memset(&cmd, 0, sizeof(struct sprdwl_cmd_info));

	return 0;
}

int wlan_cmd_mac_open(unsigned char vif_id, unsigned char mode,
		      unsigned char *mac_addr)
{
	int ret;
	struct wlan_cmd_mac_open *open = (void *)cmd.buf_send;

	open->mode = mode;
	if (NULL != mac_addr)
		memcpy((unsigned char *)(&(open->mac[0])), mac_addr, 6);

	ret =
	    wlan_cmd_send_recv(vif_id, (unsigned char *)open,
			       sizeof(struct wlan_cmd_mac_open),
			       WIFI_CMD_SET_DEV_OPEN, 8000);
	return ret;
}

int wlan_cmd_mac_close(unsigned char vif_id, unsigned char mode)
{
	struct wlan_cmd_mac_close *close = (void *)cmd.buf_send;

	close->mode = mode;
	wlan_cmd_send_recv(vif_id, (unsigned char *)(close),
			   sizeof(struct wlan_cmd_mac_close),
			   WIFI_CMD_SET_DEV_CLOSE, CMD_WAIT_TIMEOUT);
	return 0;
}

int wlan_cmd_scan(unsigned char vif_id, const unsigned char *ssid,
		  const unsigned char *channels, int len)
{
	int dataLen;
	struct wlan_cmd_scan *scan;
	unsigned char *send = (void *)cmd.buf_send;
	unsigned char *psend = send;

	unsigned char ch_num = channels[0] + 1;

	dataLen = sizeof(struct wlan_cmd_scan) + len + ch_num;

	memcpy(send, channels, ch_num);
	send += ch_num;

	scan = (struct wlan_cmd_scan *)send;
	memcpy(scan->ssid, ssid, len);
	scan->len = len;

	wlan_cmd_send_recv(vif_id, (unsigned char *)psend, dataLen,
			   WIFI_CMD_SET_SCAN, CMD_WAIT_TIMEOUT);
	return 0;
}

unsigned short sprdwl_rx_rsp_process(struct sprdwl_priv *priv,
				     unsigned char *msg)
{
	return 0;
}

unsigned short sprdwl_rx_event_process(struct sprdwl_priv *priv,
				       unsigned char *msg)
{
	return 0;
}
