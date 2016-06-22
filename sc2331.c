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
 * * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

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
#  include <nuttx/net/pkt.h>
#endif

struct sc2331_s
{
	struct net_driver_s net_dev;
};

static struct sc2331_s g_sc2331;

static int sc2331_ifup(struct net_driver_s *dev)
{
	return 0;
}

static int sc2331_ifdown(struct net_driver_s *dev)
{
	return 0;
}

static int sc2331_txavail(struct net_driver_s *dev)
{
	return 0;
}

static int sc2331_ioctl(struct net_driver_s *dev)
{
	return 0;
}

/* Initialize the SC2331 chip and driver */

int sc2331_initialize(void)
{
	struct sc2331_s *sc2331 = &g_sc2331;
	struct net_driver_s *net_dev = &(sc2331->net_dev);

	/* Register data recv interface to SDIO */

	memset(sc2331, 0, sizeof(*sc2331));

	memcpy(net_dev->d_ifname, "wlan0", 6);
	net_dev->d_ifup = sc2331_ifup;
	net_dev->d_ifdown = sc2331_ifdown;
	net_dev->d_txavail = sc2331_txavail;
	//net_dev->d_ioctl = sc2331_ioctl;
	net_dev->d_private = (void *)sc2331;

	/* Set MAC address to net_dev*/
	/* net_dev->d_mac */

	(void)netdev_register(net_dev, NET_LL_ETHERNET);

	return 0;
}

