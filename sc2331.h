/*
 * Copyright (C) 2016 Spreadtrum Communications Inc.
 *
 * Authors:
 * Eason Xiang <eason@weenas.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 * * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __SC2331_H__
#define __SC2331_H__

#include <nuttx/config.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <debug.h>
#include <errno.h>

#include <arpa/inet.h>
#include <net/ethernet.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/wdog.h>
#include <nuttx/net/arp.h>
#include <nuttx/net/netdev.h>

#ifdef CONFIG_NET_PKT
#include <nuttx/net/pkt.h>
#endif
#include "intf.h"

struct sprdwl_priv {
	struct sprdwl_if_ops *if_ops;
	struct net_driver_s net_dev;
};

#endif
