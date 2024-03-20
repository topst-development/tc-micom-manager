/****************************************************************************************
 *   FileName    : tc-key-map.c
 *   Description : Key Map c file
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited
to re-distribution in source or binary form is strictly prohibited.
This source code is provided ¡°AS IS¡± and nothing contained in this source code
shall constitute any express or implied warranty of any kind, including without limitation,
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent,
copyright or other third party intellectual property right.
No warranty is made, express or implied, regarding the information¡¯s accuracy,
completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from,
out of or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include "../inc/tc-virtual-key.h"

const TMM_Key_bridge tmm_keymap[TMM_KEY_MAX]= {
	{RSV_KEY_0,KEY_RESERVED},
	{RPT_1,KEY_1},
	{RDM_2,KEY_2},
	{NUM_3,KEY_3},
	{NUM_4,KEY_4},
	{NUM_5,KEY_5},
	{NUM_6,KEY_6},

	{RSV_KEY_7,KEY_RESERVED},
	{RSV_KEY_8,KEY_RESERVED},
	{RSV_KEY_9,KEY_RESERVED},
	{RSV_KEY_10,KEY_RESERVED},

	{ENTER,KEY_ENTER},
	{SCAN,KEY_SEARCH},
	{SETUP,KEY_SETUP},
	{MENU,KEY_HOME},
	{DISP,KEY_ESC},
	{TRACK_DOWN,KEY_LEFT},
	{TRACK_UP,KEY_RIGHT},
	{FOLDER_DOWN,KEY_DOWN},
	{FOLDER_UP,KEY_UP},
	{POWER,KEY_POWER},
	{RADIO,KEY_RADIO},
	{MEDIA,KEY_MEDIA},
	{PHONE,KEY_PHONE},
	{VOL_UP,KEY_VOLUMEUP},
	{VOL_DOWN,KEY_VOLUMEDOWN},

	{STW_MODE,KEY_MODE},
	{STW_SEEK_UP,KEY_LEFT},
	{STW_SEEK_DOWN,KEY_RIGHT},
	{STW_MUTE,KEY_MUTE},
	{STW_PHONE_ON,KEY_F7},
	{STW_PHONE_OFF,KEY_F8},
	{AP_SVM_ON, KEY_PROG3},
	{AP_SVM_OFF, KEY_PROG4},
};

const TMM_Key_bridge tmm_rotarymap[TMM_ROT_MAX]= {
	{RSV_ROT_0,KEY_RESERVED},
	{ROT_CTRL_UP,KEY_RIGHTCTRL},
	{ROT_CTRL_DOWN,KEY_LEFTCTRL},
	{ROT_VOLUP,KEY_VOLUMEUP},
	{ROT_VOLDOWN,KEY_VOLUMEDOWN},
};

