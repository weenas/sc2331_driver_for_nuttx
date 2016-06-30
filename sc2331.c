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

#include "sc2331.h"
#include "cmdevt.h"
#include "intf.h"

static struct sprdwl_priv g_priv;

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

static int sc2331_ioctl(struct net_driver_s *dev, int cmd, long arg)
{
	return 0;
}

/* Initialize the SC2331 chip and driver */

int sc2331_initialize(void)
{
	struct sprdwl_priv *priv = &g_priv;
	struct net_driver_s *net_dev = &(priv->net_dev);

	memset(priv, 0, sizeof(*priv));

	/* Register data recv interface to SDIO */
	sprdwl_intf_init(priv);

	sprdwl_cmd_init();

	memcpy(net_dev->d_ifname, "wlan0", 6);
	net_dev->d_ifup = sc2331_ifup;
	net_dev->d_ifdown = sc2331_ifdown;
	net_dev->d_txavail = sc2331_txavail;
	net_dev->d_ioctl = sc2331_ioctl;
	net_dev->d_private = (void *)priv;

	/* Set MAC address to net_dev*/
	/* net_dev->d_mac */

	(void)netdev_register(net_dev, NET_LL_ETHERNET);

	return 0;
}

