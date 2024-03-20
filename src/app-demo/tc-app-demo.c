/****************************************************************************************
 *   FileName    : tc-demo-app.c
 *   Description : appliation demo c file
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/reboot.h>

#ifdef HAVE_DBUS 
#include "DBusMsgDef.h"
#include "tc-micom-dbusmanager.h"
#endif
#include "tc-micom-log.h"
#include "tc-micom-protocol.h"
#include "tc-ipc-ctrl.h"
#include "tc-app-demo.h"
#include "tc-micom-manager.h"
#include "tc-virtual-key.h"

#define RVC_CAMERA_SWITCH_TYPE_PATH 	"/sys/devices/platform/switch1/switch_type"
#define RVC_CAMERA_SWITCH_NUMBER_PATH 	"/sys/devices/platform/switch1/switch_status"

static void tmm_rvc_ipc_send(uint32_t svm_cmd, uint8_t sub_cmd);
static void tmm_reset_ipc_send(void);
static void tmm_panic_ipc_send(void);
static void tmm_change_camera_mode_ipc_send(void);

#if 0
static void tmm_camera_switch_control(const char *switch_path, int isOn);

static void tmm_camera_switch_control(const char *switch_path, int isOn)
{
	int switch_fd;
	int ret;
	char switch_off[1] = {48};
	char switch_on[1] = {49};

	DEBUG_TMM_PRINTF("%s : %d \n", switch_path, isOn)
	switch_fd = open(switch_path, O_RDWR);
	if(switch_fd > -1)
	{
		if(isOn == 0)
		{
			ret = write(switch_fd,switch_off, sizeof(char));
			if(ret != 1)
			{
				ERROR_TMM_PRINTF("Can't write 0 to %s, %d\n", switch_path, ret);
			}
		}
		else if(isOn == 1)
		{
			ret = write(switch_fd, switch_on, sizeof(char));
			if(ret != 1)
			{
				ERROR_TMM_PRINTF("Can't write 1 to %s, %d\n", switch_path,ret);
			}
		}
		else
		{
		}
		close(switch_fd);
	}
	else
	{
		ERROR_TMM_PRINTF("Can't open switch file. \n");
		ERROR_TMM_PRINTF("%s : %d\n",switch_path, switch_fd);
	}
}
#endif

static void tmm_rvc_ipc_send(uint32_t svm_cmd, uint8_t sub_cmd)
{
	uint16_t cmd1=0, cmd2=0, length=0;
	cmd1 = (uint16_t)TMM_IPC_COMMAND_RVC;

	if(svm_cmd == (uint32_t)TMM_IPC_RVC_ONOFF)
	{
		unsigned char data;

		cmd2 = (uint16_t)TMM_IPC_RVC_ONOFF;
		if(sub_cmd == (uint8_t)RVC_ON)
		{
			length = 1;
			data = RVC_ON;
			INFO_TMM_PRINTF("Send RVC On\n");
			(void)tc_ipc_ctrl_msg_send(ap_ipc_handle,cmd1, cmd2, &data, length);
		}
		else if(sub_cmd == (uint8_t)RVC_OFF)
		{
			length = 1;
			data = RVC_OFF;
			INFO_TMM_PRINTF("Send RVC OFF\n");
			(void)tc_ipc_ctrl_msg_send(ap_ipc_handle,cmd1, cmd2, &data, length);
		}
		else
		{
			;
		}
	}
	else if(svm_cmd == (uint32_t)TMM_IPC_RVC_MODE)
	{
		cmd2 = (uint16_t)TMM_IPC_RVC_MODE;
		/* reserved command */
		;
	}
	else
	{
		;
	}

}

static void tmm_reset_ipc_send(void)
{
	uint16_t cmd1=0, cmd2=0, length=0;
	cmd1 = (uint16_t)TMM_IPC_COMMAND_RESET;
	cmd2 = RESET_START;

	(void)tc_ipc_ctrl_msg_send(ap_ipc_handle,cmd1, cmd2, NULL, length);
}

static void tmm_panic_ipc_send(void)
{
	uint16_t cmd1=0, cmd2=0, length=0;
	cmd1 = (uint16_t)TMM_IPC_COMMAND_PANIC;
	cmd2 = (uint16_t)0;

	(void)tc_ipc_ctrl_msg_send(ap_ipc_handle,cmd1, cmd2, NULL, length);
}

static void tmm_change_camera_mode_ipc_send(void)
{
	uint16_t cmd1=0, cmd2=0, length=0;
	cmd1 = (uint16_t)TMM_IPC_COMMAND_CHAGNE_CAMERA_MODE;
	cmd2 = (uint16_t)0;

	(void)tc_ipc_ctrl_msg_send(ap_ipc_handle,cmd1, cmd2, NULL, length);
}

void tmm_rvc_off_method_received(void)
{
	tmm_rvc_ipc_send(TMM_IPC_RVC_ONOFF,RVC_OFF);
}

void tmm_rvc_on_method_received(void)
{
	tmm_rvc_ipc_send(TMM_IPC_RVC_ONOFF,RVC_ON);
}

void tmm_reset_method_received(void)
{
	uint32_t ret;

	tmm_reset_ipc_send();
	usleep(100000);

	ret = reboot(RB_AUTOBOOT);
	if (ret == 0) {
		ERROR_TMM_PRINTF("Reset Command  fail(0x%08x)\n", ret);
	}
}

void tmm_robust_method_received(void)
{
	tmm_panic_ipc_send();
	usleep(100000);
	system("echo c > /proc/sysrq-trigger");
}

/*
	tmm_change_camera_mode_method_received()
		direction : main core -> sub core
		action : send change_camera_mode command to ipc
*/
void tmm_change_camera_mode_method_received(void)
{
	DEBUG_TMM_PRINTF("In\n");
	tmm_change_camera_mode_ipc_send();
}

/*
	tmm_rvc_ipc_received()
	RVC On
		direction : main core -> sub core
		action : RVC switch enable
	RVC Off
		direction : sub core -> Main core
		action : send RVC_OFF signal
*/
void tmm_rvc_ipc_received(uint16_t cmd, unsigned char *buf, uint32_t length)
{
	if(cmd == TMM_IPC_RVC_ONOFF)
	{
		if(length == 1)
		{
			unsigned char sub_cmd;
			if(buf != NULL)
			{
				sub_cmd = buf[0];
				if(sub_cmd == RVC_ON)
				{
					INFO_TMM_PRINTF("Receive IPC RVC_ON from Opposite Core\n");
#ifdef HAVE_DBUS
					tmm_dbus_emit_signal(SignalMicomManagerRVCOn);
#endif
				}
				else if (sub_cmd == RVC_OFF)
				{
					INFO_TMM_PRINTF("Receive IPC RVC_OFF from Opposite Core\n");
#ifdef HAVE_DBUS
					tmm_dbus_emit_signal(SignalMicomManagerRVCOff);
#endif
				}
				else
				{
					ERROR_TMM_PRINTF("Not define RVC sub command(%d)\n", length);
				}
			}
			else
			{
				ERROR_TMM_PRINTF("RVC Command lenght fail(%d)\n", length);
			}
		}
		else
		{
			ERROR_TMM_PRINTF("RVC Command lenght fail(%d)\n", length);
		}

	}
	else if(cmd == TMM_IPC_RVC_MODE)
	{
		/* reserved */
		;
	}
	else
	{
		;
	}
}

void tmm_reset_ipc_received(uint16_t cmd)
{
	if(cmd == RESET_START)
	{
		uint32_t ret;

		ret = reboot(RB_AUTOBOOT);
		if (ret == 0) {
			ERROR_TMM_PRINTF("Reset Command  fail(0x%08x)\n", ret);
		}
	}
}

void tmm_panic_ipc_received(void)
{
	system("echo c > /proc/sysrq-trigger");
}

void tmm_change_camera_mode_ipc_received(void)
{
#ifdef HAVE_DBUS
	tmm_dbus_emit_signal(SignalMicomManagerChangeCameraMode);
#endif
}



