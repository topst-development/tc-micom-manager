/****************************************************************************************
 *   FileName    : tc-virtual_key.h
 *   Description : Virtual Key Header File
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

#ifndef TMM_VIKEY_H
#define TMM_VIKEY_H

typedef  enum {
	RSV_KEY_0 =0, 		//reserved 0x00
	RPT_1,
	RDM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,

	RSV_KEY_7,
	RSV_KEY_8,
	RSV_KEY_9,
	RSV_KEY_10,
	RSV_KEY_11,
	RSV_KEY_12,
	RSV_KEY_13,
	RSV_KEY_14,
	RSV_KEY_15,
	RSV_KEY_16,

	ENTER,			//17
	SCAN,
	SETUP,
	MENU,
	DISP,			//21
	TRACK_DOWN,
	TRACK_UP,
	FOLDER_DOWN,
	FOLDER_UP,
	POWER,
	RADIO,
	MEDIA,
	PHONE,
	VOL_UP,			//30
	VOL_DOWN,

	STW_MODE,		//32
	STW_SEEK_UP,
	STW_SEEK_DOWN,
	STW_MUTE,
	STW_PHONE_ON,
	STW_PHONE_OFF,
	RSV_KEY_HOME,
	RSV_KEY_RETURN,
	RSV_KEY_PLAYPAUSE,	//40
	RSV_KEY_CARPLAY,
	RSV_KEY_42,
	RSV_KEY_43,
	RSV_KEY_44,
	RSV_KEY_45,
	RSV_KEY_46,
	RSV_KEY_47,
	RSV_KEY_48,
	RSV_KEY_49,

	AP_SVM_ON,		//50 (Main<->sub interface)
	AP_SVM_OFF,

	TMM_KEY_MAX
} TMM_KEY;

typedef  enum {
	RSV_ROT_0 =0, 		//reserved 0x00
	ROT_CTRL_UP,
	ROT_CTRL_DOWN,
	ROT_VOLUP,
	ROT_VOLDOWN,
	TMM_ROT_MAX,
} TMM_ROTARY;

typedef  enum {
	TMM_KEY_SIMPLE =0x00,
	TMM_KEY_PRESS = 0x01,
	TMM_KEY_RELEASE = 0x02
} TMM_KEY_STATUS;


typedef struct
{
	uint32_t reciveKey;
	uint32_t virtualKey;
} TMM_Key_bridge;


#if 0
#define KEY_1 				0x01
#define KEY_2				0x02
#define KEY_3				0x03
#define KEY_4				0x04
#define KEY_5				0x05
#define KEY_6                           0x06
#define KEY_ENTER			0x11
#define KEY_SCAN			0x12
#define KEY_SETUP			0x13
#define KEY_MENU			0x14
#define KEY_DISP				0x15
#define KEY_TRACKDOWN             0x16
#define KEY_TRACKUP                  0x17
#define KEY_FOLDERDOWN           0x18
#define KEY_FOLDERUP                0x19
#define KEY_POWER                    0x1a
#define KEY_RADIO                     0x1b
#define KEY_MEDIA                     0x1c
#define KEY_PHONE                     0x1d
#define KEY_VOLUMEUP               0x1e
#define KEY_VOLUMEDOWN          0x1f
#define KEY_MODE                      0x20
#define KEY_SEEKUP                     0x21
#define KEY_SEEKDOWN                0x22
#define KEY_MUTE                       0x23
#define KEY_PHONEON                  0x24
#define KEY_PHONEOFF                 0x25

#endif

const TMM_Key_bridge tmm_keymap[TMM_KEY_MAX];
const TMM_Key_bridge tmm_rotarymap[TMM_ROT_MAX];
void tmm_virtual_key_open(void);
void tmm_virtual_key_close(void);
void tmm_virtual_key_search(unsigned char key_id, unsigned char status);
void tmm_virtual_rotary_search(unsigned char key_id);

#endif /* TMM_VIKEY_H */

