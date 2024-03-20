/****************************************************************************************
 *   FileName    : tc-micom-manager.c
 *   Description : micom manager c file
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
#include <stdint.h>
#ifdef HAVE_DBUS 
#include "DBusMsgDef.h"
#include "tc-micom-dbusmanager.h"
#endif
#include "tc-micom-log.h"
#include "tc-micom-protocol.h"
#include "tc-virtual-key.h"
#include "tc-micom-manager.h"
#include "tc-ipc-ctrl.h"
#include "tc-app-demo.h"
#ifdef HAVE_SDM_DEMO
#include "tc-sdm-demo.h"
#endif


#define TMM_IPC_COMMAND_VEHICLE_DEMO        4
#define TMM_IPC_COMMAND_VEHICLE_TIME        5

void * micom_ipc_handle = NULL;
void * ap_ipc_handle = NULL;

static void tmm_request_key(ipc_request_key_type host_type);

static void tc_parser_receiveddata(IPC_msg *receivedData)
{
	switch(receivedData->cmd1)
	{
		case TMM_IPC_COMMAND_KEY:
			if(receivedData->cmd2 == TMM_IPC_PUSH_KEY)
			{
				DEBUG_TMM_PRINTF("Received push key from MICOM : key(0x%02x), status(0x%02x)\n", receivedData->data[0],receivedData->data[1]);
				if(receivedData->data[1] == TMM_IPC_KEY_PRESS)
				{
					tmm_virtual_key_search(receivedData->data[0], TMM_KEY_PRESS);
				}
				else
				{
					tmm_virtual_key_search(receivedData->data[0], TMM_KEY_RELEASE);
				}
			}
			else if(receivedData->cmd2 == TMM_IPC_ROTARY_KEY)
			{
				DEBUG_TMM_PRINTF("Received Rotary key from MICOM : 0x%02x\n", receivedData->data[0]);
				tmm_virtual_rotary_search(receivedData->data[0]);
			}
			else
			{
				DEBUG_TMM_PRINTF("cmd1 is (%d) but, cmd2 (%d) is invalid type.\n", receivedData->cmd1, receivedData->cmd2);
			}
			break;
		case TMM_IPC_COMMAND_POWER:
			DEBUG_TMM_PRINTF("Received power off singal\n");
			system("halt");
			break;
		case TMM_IPC_COMMAND_NVM:
		case TMM_IPC_COMMAND_FIRMWARE:
		case TMM_IPC_COMMAND_VEHICLE :
		case TMM_IPC_COMMAND_SVM:
				DEBUG_TMM_PRINTF("Don't support this command (%d)\n",receivedData->cmd1);
			break;

		case TMM_IPC_COMMAND_HEARTBIT:
			if(receivedData->cmd2 == TMM_IPC_HEARTBIT_START)
			{
				unsigned char send_data[3];
				DEBUG_TMM_PRINTF("Received system monitoring start singal\n");
				/* interval  0x3e8 = 1000ms*/
				send_data[0]=0x3;
				send_data[1]=0xE8;
				/* max retry 3 */
				send_data[2]=3;
				tc_ipc_ctrl_msg_send(micom_ipc_handle ,TMM_IPC_COMMAND_HEARTBIT, TMM_IPC_HEARTBIT_START, send_data,3);
			}
			else if(receivedData->cmd2 == TMM_IPC_HEARTBIT_OK)
			{
				DEBUG_TMM_PRINTF("Received system monitoring singal\n");
				tc_ipc_ctrl_msg_send(micom_ipc_handle,TMM_IPC_COMMAND_HEARTBIT, TMM_IPC_HEARTBIT_OK, NULL, 0 );
			}
			break;
	#ifdef HAVE_SDM_DEMO
		case TMM_IPC_COMMAND_SDM:
				tmm_sdm_ipc_received(receivedData->cmd2, receivedData->data, receivedData->length);
			break;
	#endif
		case TMM_IPC_COMMAND_RESET:
				tmm_reset_ipc_received(receivedData->cmd2);
			break;
		case TMM_IPC_COMMAND_PANIC:
				tmm_panic_ipc_received();
			break;
		case TMM_IPC_COMMAND_RVC:
				tmm_rvc_ipc_received(receivedData->cmd2, receivedData->data, receivedData->length);
			break;
		case TMM_IPC_COMMAND_CHAGNE_CAMERA_MODE:
				tmm_change_camera_mode_ipc_received();
			break;
		default :
			{
				DEBUG_TMM_PRINTF("Receive unknown data. cmd1(0x%04x), cmd2(0x%04x)\n", receivedData->cmd1, receivedData->cmd2);
			}
			break;
	}
}

int32_t tmm_initialize(const char * micom_device, const char * ap_device,ipc_request_key_type host_type)
{
	int32_t ret=0;

	tc_ipc_ctrl_debug(g_tmm_debug);

	tmm_virtual_key_open();

	if(micom_device != NULL)
	{
		micom_ipc_handle = tc_ipc_ctrl_open(micom_device, tc_parser_receiveddata);
		if(micom_ipc_handle == NULL)
		{
			ERROR_TMM_PRINTF("Cat not open : %s\n", micom_device);
			ret = -1;
		}
	}

	if((ap_device != NULL)&&(ret == 0))
	{
		ap_ipc_handle = tc_ipc_ctrl_open(ap_device, tc_parser_receiveddata);
		if(ap_ipc_handle == NULL)
		{
			ERROR_TMM_PRINTF("Cat not open : %s\n", ap_device);
			ret = -1;
		}
	}

	/* 3 . dbus method */
#ifdef HAVE_DBUS
	tmm_dbus_initialize();
#endif

	tmm_request_key(host_type);

	return ret;
}

void tmm_release(void)
{
	DEBUG_TMM_PRINTF("\n");

	if(micom_ipc_handle != NULL)
	{
		tc_ipc_ctrl_close(micom_ipc_handle);
		micom_ipc_handle = NULL;
	}

	if(ap_ipc_handle != NULL)
	{
		tc_ipc_ctrl_close(ap_ipc_handle);
		ap_ipc_handle = NULL;
	}

#ifdef HAVE_DBUS
	tmm_dbus_release();
#endif
	tmm_virtual_key_close();

}

static void tmm_request_key(ipc_request_key_type host_type)
{
	uint64_t key_bitmap;

	DEBUG_TMM_PRINTF("host type(%d)\n", host_type);
	if(host_type == eRequest_Key_SVM)
	{
		key_bitmap = (uint64_t )EXTKEY_POWER_BIT;
	}
	else if(host_type == eRequest_Key_IVI)
	{
		key_bitmap = (uint64_t )EXTKEY_POWER_BIT;
		key_bitmap = ~key_bitmap;
	}
	else
	{
		key_bitmap = (uint64_t )0xFFFFFFFFFFFFFFFF;
	}

	if(micom_ipc_handle != NULL)
	{
		tc_ipc_ctrl_msg_send(micom_ipc_handle,TMM_IPC_COMMAND_KEY, TMM_IPC_REQUEST_KEY, (unsigned char *)&key_bitmap, 8);
	}
}

int32_t tmm_resume(const char * micom_device, const char * ap_device,ipc_request_key_type host_type)
{
	int32_t ret=0;

	if(micom_device != NULL)
	{
		micom_ipc_handle = tc_ipc_ctrl_open(micom_device, tc_parser_receiveddata);
		if(micom_ipc_handle == NULL)
		{
			ERROR_TMM_PRINTF("Cat not open : %s\n", micom_device);
			ret = -1;
		}
	}

	if((ap_device != NULL)&&(ret == 0))
	{
		ap_ipc_handle = tc_ipc_ctrl_open(ap_device, tc_parser_receiveddata);
		if(ap_ipc_handle == NULL)
		{
			ERROR_TMM_PRINTF("Cat not open : %s\n", ap_device);
			ret = -1;
		}
	}

	tmm_request_key(host_type);

	return ret;
}

void tmm_suspend(void)
{
	DEBUG_TMM_PRINTF("\n");

	if(micom_ipc_handle != NULL)
	{
		tc_ipc_ctrl_close(micom_ipc_handle);
		micom_ipc_handle = NULL;
	}

	if(ap_ipc_handle != NULL)
	{
		tc_ipc_ctrl_close(ap_ipc_handle);
		ap_ipc_handle = NULL;
	}
}

