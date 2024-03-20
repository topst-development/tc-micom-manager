/****************************************************************************************
 *   FileName    : tc-micom-dbusmanager.c
 *   Description : dbus manager c file
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

#include "tc-micom-log.h"
#include "TCDBusRawAPI.h"
#include "DBusMsgDef.h"
#include "tc-micom-dbusmanager.h"
#include "tc-micom-manager.h"
#include "tc-app-demo.h"
#include "tc-micom-protocol.h"

typedef void (*DBusMethodCallFunction)(DBusMessage *message);

static void tmm_dbus_method_process(uint32_t id, DBusMessage *message)
{
	if(id == MethodMicomManagerRVCOn)
	{
		if(GetArgumentFromDBusMessage(message,
										DBUS_TYPE_INVALID))
		{
			tmm_rvc_on_method_received();
		}
	}
	else if(id == MethodMicomManagerRVCOff)
	{
		if(GetArgumentFromDBusMessage(message,
										DBUS_TYPE_INVALID))
		{
			tmm_rvc_off_method_received();
		}
	}
	else if(id == MethodMicomManagerReset)
	{
		if(GetArgumentFromDBusMessage(message,
										DBUS_TYPE_INVALID))
		{
			tmm_reset_method_received();
		}
	}
	else if(id == MethodMicomManagerRobust)
	{
		if(GetArgumentFromDBusMessage(message,
										DBUS_TYPE_INVALID))
		{
			tmm_robust_method_received();
		}
	}
	else if(id == MethodMicomManagerChangeCameraMode)
	{
		if(GetArgumentFromDBusMessage(message,
										DBUS_TYPE_INVALID))
		{
			tmm_change_camera_mode_method_received();
		}
	}
	else
	{
		//
	}
}

static DBusMsgErrorCode OnReceivedMethodCall(DBusMessage *message, const char *interface)
{
	DBusMsgErrorCode error = ErrorCodeNoError;

	if (interface != NULL &&
		strcmp(interface, MICOMMANAGER_EVENT_INTERFACE) == 0)
	{
		uint32_t index;
		uint32_t stop = 0;
		for (index = MethodMicomManagerRVCOn; index < TotalMethodMicomManagerEvents && !stop; index++)
		{
			if (dbus_message_is_method_call(message,
											MICOMMANAGER_EVENT_INTERFACE,
											g_methodMicomManagerEventNames[index]))
			{
				tmm_dbus_method_process(index, message);
				stop = 1;
			}
		}
		if(!stop)
		{
			error = ErrorCodeUnknown;
		}
	}

	return error;
}


void tmm_dbus_emit_signal(uint32_t name)
{
	if (name < TotalSignalMicomManagerEvents)
	{
		DBusMessage *message;
		message = CreateDBusMsgSignal(MICOMMANAGER_PROCESS_OBJECT_PATH, MICOMMANAGER_EVENT_INTERFACE,
									  g_signalMicomManagerEventNames[name],
									  DBUS_TYPE_INVALID);
		if (message != NULL)
		{
			if (SendDBusMessage(message, NULL))
			{
				//debug message
				DEBUG_TMM_PRINTF("%s %s \n", __FUNCTION__, g_signalMicomManagerEventNames[name]);
			}
			else
			{
				ERROR_TMM_PRINTF("%s: SendDBusMessage failed\n", __FUNCTION__);
			}

			dbus_message_unref(message);
		}
		else
		{
			ERROR_TMM_PRINTF("%s: CreateDBusMsgSignal failed\n", __FUNCTION__);
		}
	}
}

void tmm_dbus_emit_signal_data(uint32_t name, int32_t data)
{
	if (name < TotalSignalMicomManagerEvents)
	{
		DBusMessage *message;
		message = CreateDBusMsgSignal(MICOMMANAGER_PROCESS_OBJECT_PATH, MICOMMANAGER_EVENT_INTERFACE,
									  g_signalMicomManagerEventNames[name],
									  DBUS_TYPE_INT32, &data,
									  DBUS_TYPE_INVALID);
		if (message != NULL)
		{
			if (SendDBusMessage(message, NULL))
			{
				//debug message
				DEBUG_TMM_PRINTF("%s %s : data : %d \n", __FUNCTION__, g_signalMicomManagerEventNames[name], data);
			}
			else
			{
				ERROR_TMM_PRINTF("%s: SendDBusMessage failed\n", __FUNCTION__);
			}

			dbus_message_unref(message);
		}
		else
		{
			ERROR_TMM_PRINTF("%s: CreateDBusMsgSignal failed\n", __FUNCTION__);
		}
	}
}

void tmm_dbus_emit_signal_char(uint32_t name, char data)
{
	if (name < TotalSignalMicomManagerEvents)
	{
		DBusMessage *message;
		message = CreateDBusMsgSignal(MICOMMANAGER_PROCESS_OBJECT_PATH, MICOMMANAGER_EVENT_INTERFACE,
									  g_signalMicomManagerEventNames[name],
									  DBUS_TYPE_BYTE, &data,
									  DBUS_TYPE_INVALID);

		if (message != NULL)
		{
			if (SendDBusMessage(message, NULL))
			{
				//debug message
				DEBUG_TMM_PRINTF("%s %s : data : %d \n", __FUNCTION__, g_signalMicomManagerEventNames[name], data);
			}
			else
			{
				ERROR_TMM_PRINTF("%s: SendDBusMessage failed\n", __FUNCTION__);
			}

			dbus_message_unref(message);
		}
		else
		{
			ERROR_TMM_PRINTF("%s: CreateDBusMsgSignal failed\n", __FUNCTION__);
		}
	}
}

void tmm_dbus_initialize(void)
{
	SetDBusPrimaryOwner(MICOMMANAGER_PROCESS_DBUS_NAME);
	SetCallBackFunctions(NULL, OnReceivedMethodCall);
	AddMethodInterface(MICOMMANAGER_EVENT_INTERFACE);
	InitializeRawDBusConnection("MICOM MANAGER DBUS");
}

void tmm_dbus_release(void)
{
	ReleaseRawDBusConnection();
}
