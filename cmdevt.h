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

#ifndef _CMDEVT_H__
#define _CMDEVT_H__

#define BUF_SEND_SIZE (512)
#define BUF_RECV_SIZE (512)

struct sprdwl_cmd_info {
	unsigned char	buf_send[BUF_SEND_SIZE];
	unsigned char	buf_recv[BUF_RECV_SIZE];
};

enum ITM_HOST_TROUT3_CMD_TYPE {
	HOST_SC2331_CMD = 0,
	SC2331_HOST_RSP,
	HOST_SC2331_PKT,
	HOST_SC2331_WAPI,
};

enum ITM_HOST_TROUT3_CMD_LIST {
	WIFI_CMD_GET_MODE = 1,
	WIFI_CMD_GET_RSSI,
	WIFI_CMD_GET_TXRATE_TXFAILED,
	WIFI_CMD_SET_SCAN,
	WIFI_CMD_SET_AUTH_TYPE,
	WIFI_CMD_SET_WPA_VERSION,
	WIFI_CMD_SET_PAIRWISE_CIPHER,
	WIFI_CMD_SET_GROUP_CIPHER,
	WIFI_CMD_SET_AKM_SUITE,
	WIFI_CMD_SET_CHANNEL,	/*10-0xA */
	WIFI_CMD_SET_BSSID,
	WIFI_CMD_SET_ESSID,
	WIFI_CMD_KEY_ADD,
	WIFI_CMD_KEY_DEL,
	WIFI_CMD_KEY_SET,
	WIFI_CMD_SET_DISCONNECT,
	WIFI_CMD_SET_RTS_THRESHOLD,
	WIFI_CMD_SET_FRAG_THRESHOLD,
	WIFI_CMD_SET_PMKSA,
	WIFI_CMD_DEL_PMKSA,	/*20--0x14 */
	WIFI_CMD_FLUSH_PMKSA,
	WIFI_CMD_SET_DEV_OPEN,
	WIFI_CMD_SET_DEV_CLOSE,
	WIFI_CMD_SET_PSK,
	WIFI_CMD_START_BEACON,
	WIFI_CMD_SET_WPS_IE,
	WIFI_CMD_TX_MGMT,
	WIFI_CMD_REMAIN_CHAN,
	WIFI_CMD_CANCEL_REMAIN_CHAN,
	WIFI_CMD_P2P_IE,	/*30---0x1e */
	WIFI_CMD_CHANGE_BEACON,
	WIFI_CMD_REGISTER_FRAME,
	WIFI_CMD_NPI_MSG,
	WIFI_CMD_NPI_GET,
	WIFI_CMD_SET_FT_IE,
	WIFI_CMD_UPDATE_FT_IE,
	WIFI_CMD_ASSERT,
	WIFI_CMD_SLEEP,
	WIFI_CMD_ADD_SOFTAP_BLACKLIST,
	WIFI_CMD_DEL_SOFTAP_BLACKLIST,
	WIFI_CMD_SCAN_NOR_CHANNELS,
	WIFI_CMD_GET_IP,
	WIFI_CMD_REQ_LTE_CONCUR,
	WIFI_CMD_SET_CQM_RSSI,
	WIFI_CMD_MULTICAST_FILTER,
	WIFI_CMD_DISASSOC,
	WIFI_CMD_MAX,

	WIFI_EVENT_CONNECT = 128,
	WIFI_EVENT_DISCONNECT,
	WIFI_EVENT_SCANDONE,
	WIFI_EVENT_MGMT_DEAUTH,
	WIFI_EVENT_MGMT_DISASSOC,
	WIFI_EVENT_REMAIN_ON_CHAN_EXPIRED,
	WIFI_EVENT_NEW_STATION,
	WIFI_EVENT_REPORT_FRAME,
	WIFI_EVENT_CONNECT_AP,
	WIFI_EVENT_SDIO_SEQ_NUM,
	WIFI_EVENT_REPORT_SCAN_FRAME,
	WIFI_EVENT_REPORT_MIC_FAIL,
	WIFI_EVENT_REPORT_CQM_RSSI_LOW,
	WIFI_EVENT_REPORT_CQM_RSSI_HIGH,
	WIFI_EVENT_REPORT_CQM_RSSI_LOSS_BEACON,
	WIFI_EVENT_MLME_TX_STATUS,
	WIFI_EVENT_REPORT_VERSION,
	WIFI_EVENT_MAX,
};

/* The reason code is defined by CP2 */
enum wlan_cmd_disconnect_reason {
	AP_LEAVING = 0xc1,
	AP_DEAUTH = 0xc4,
};

struct wlan_cmd_add_key {
	unsigned char mac[6];
	unsigned char keyseq[8];
	unsigned char pairwise;
	unsigned char cypher_type;
	unsigned char key_index;
	unsigned char key_len;
	unsigned char value[0];
} __attribute__ ((packed));

struct wlan_cmd_del_key {
	unsigned char key_index;
	unsigned char pairwise;	/* unicase or group */
	unsigned char mac[6];
} __attribute__ ((packed));

struct wlan_cmd_beacon {
	unsigned char len;
	unsigned char value[0];
} __attribute__ ((packed));

struct wlan_cmd_mac_open {
	unsigned short mode;	/* AP or STATION mode */
	unsigned char mac[6];
} __attribute__ ((packed));

struct wlan_cmd_mac_close {
	unsigned char mode;	/* AP or STATION mode */
} __attribute__ ((packed));

struct wlan_cmd_scan_ssid {
	unsigned char len;
	unsigned char ssid[0];
} __attribute__ ((packed));

struct wlan_cmd_set_key {
	unsigned int key_index;
} __attribute__ ((packed));

struct wlan_cmd_disconnect {
	unsigned short reason_code;
} __attribute__ ((packed));

struct wlan_cmd_set_essid {
	unsigned short len;
	unsigned char essid[0];
} __attribute__ ((packed));

struct wlan_cmd_set_bssid {
	unsigned char addr[6];
} __attribute__ ((packed));

struct wlan_cmd_set_channel {
	unsigned int channel;
} __attribute__ ((packed));

struct wlan_cmd_set_key_management {
	unsigned int key_mgmt;

} __attribute__ ((packed));

struct wlan_cmd_set_cipher {
	unsigned int cipher;
} __attribute__ ((packed));

struct wlan_cmd_set_auth_type {
	unsigned int type;

} __attribute__ ((packed));

struct wlan_cmd_scan {
	/*
	   unsigned char channel_num;
	   unsigned char channel[15];
	 */
	unsigned int len;
	unsigned char ssid[0];
} __attribute__ ((packed));

typedef struct {
	unsigned char ops;
	unsigned short channel;
	signed short signal;
	unsigned short frame_len;
} wlan_event_scan_rsp_t;

struct wlan_event_mic_failure {
	unsigned char key_id;
	unsigned char is_mcast;
} __attribute__ ((packed));

struct wlan_cmd_disassoc {
	unsigned char mac[6];
	unsigned short reason_code;
} __attribute__ ((packed));

int sprdwl_cmd_init(void);

int wlan_cmd_mac_open(unsigned char vif_id, unsigned char mode,
		      unsigned char *mac_addr);
int wlan_cmd_mac_close(unsigned char vif_id, unsigned char mode);
int wlan_cmd_scan(unsigned char vif_id, const unsigned char *ssid,
		  const unsigned char *channels, int len);
#endif

