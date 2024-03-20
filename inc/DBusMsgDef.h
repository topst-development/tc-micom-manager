/****************************************************************************************
 *   FileName    : DBusMsgDef.h
 *   Description : DBus message define Header File
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
#ifndef DBUS_MSG_DEF_H
#define DBUS_MSG_DEF_H

#define MICOMMANAGER_PROCESS_DBUS_NAME							"telechips.micom_manager.process"
#define MICOMMANAGER_PROCESS_OBJECT_PATH						"/telechips/micom_manager/process"
#define MICOMMANAGER_EVENT_INTERFACE							"micom_manager.event"

// MICOM_MANAGER SIGNAL EVENT DEFINES
#define SIGNAL_MICOM_MANAGER_RVC_ON                            "signal_micom_manager_rvc_on"
#define SIGNAL_MICOM_MANAGER_RVC_OFF                           "signal_micom_manager_rvc_off"
#define SIGNAL_MICOM_MANAGER_CHANGE_CAMERA_MDOE                "signal_micom_manager_change_camera_mode"

typedef enum {
	SignalMicomManagerRVCOn,
	SignalMicomManagerRVCOff,
	SignalMicomManagerChangeCameraMode,
	TotalSignalMicomManagerEvents
} SignalMicomManagerEvent;

extern const char *g_signalMicomManagerEventNames[TotalSignalMicomManagerEvents];


// MICOM_MANAGER METHOD EVENT DEFINES
#define METHOD_MICOM_MANAGER_RVC_ON					"method_micom_manager_rvc_on"
#define METHOD_MICOM_MANAGER_RVC_OFF					"method_micom_manager_rvc_off"
#define METHOD_MICOM_MANAGER_RESET					"method_micom_manager_reset"
#define METHOD_MICOM_MANAGER_ROBUST					"method_micom_manager_robust"
#define METHOD_MICOM_MANAGER_CHANGE_CAMERA_MODE			"method_micom_manager_change_camera_mode"

typedef enum {
	MethodMicomManagerRVCOn,
	MethodMicomManagerRVCOff,
	MethodMicomManagerReset,
	MethodMicomManagerRobust,
	MethodMicomManagerChangeCameraMode,
	TotalMethodMicomManagerEvents
} MethodMicomManagerEvent;

extern const char *g_methodMicomManagerEventNames[TotalMethodMicomManagerEvents];


#endif
