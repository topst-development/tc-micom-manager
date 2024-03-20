/****************************************************************************************
 *   FileName    : tc-micom-protocol.h
 *   Description : Micom Protocol Header File
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

#ifndef TMM_IPC_PROTOCOL_H
#define TMM_IPC_PROTOCOL_H

#define EXTKEY_POWER_BIT 1<<15		// SVN MODE KEY for 803x

typedef enum
{
	TMM_IPC_COMMAND_POWER = 0x01,
	TMM_IPC_COMMAND_KEY,
	TMM_IPC_COMMAND_NVM,
	TMM_IPC_COMMAND_HEARTBIT,
	TMM_IPC_COMMAND_VEHICLE,
	TMM_IPC_COMMAND_FIRMWARE,
	TMM_IPC_COMMAND_USERAPP,
	TMM_IPC_COMMAND_CUSTOMER,
	TMM_IPC_COMMAND_SVM,
	TMM_IPC_COMMAND_RESET,
	TMM_IPC_COMMAND_PANIC,
	TMM_IPC_COMMAND_RVC,
	TMM_IPC_COMMAND_CHAGNE_CAMERA_MODE,
#ifdef HAVE_SDM_DEMO
	TMM_IPC_COMMAND_SDM = 0xFFFE,
#endif
	TMM_IPC_COMMAND_MAX
}TMM_IPC_COMMAND_TYPE;


typedef enum
{
	TMM_IPC_VEHICLE_POWER = 0x10,
	TMM_IPC_VEHICLE_SPEED,
	TMM_IPC_VEHICLE_ILLUMINATION,
	TMM_IPC_VEHICLE_REVERSE,
	TMM_IPC_VEHICLE_CLIMATE,
	TMM_IPC_VEHICLE_TIMESTAMP,
	TMM_IPC_VEHICLE_LAUNCH,
	TMM_IPC_VEHICLE_MAX
}TMM_IPC_VEHICLE_TYPE;

typedef enum
{
	TMM_IPC_CLIMATE_AUTO = 0x01,
	TMM_IPC_CLIMATE_TEMPERATURE,
	TMM_IPC_CLIMATE_POWER,
	TMM_IPC_CLIMATE_AIR_CONDITION,
	TMM_IPC_CLIMATE_AIR_DIRECTOR,
	TMM_IPC_CLIMATE_AIR_STREAM,
	TMM_IPC_CLIMATE_FRONT_DEFROST,
	TMM_IPC_CLIMATE_REAR_DEFROST,
	TMM_IPC_CLIMATE_MAX
}TMM_IPC_CLIMATE_TYPE;

typedef enum
{
	TMM_IPC_TIMESTAMP_MICOM = 0x01,
	TMM_IPC_TIMESTAMP_TSOUND,
	TMM_IPC_TIMESTAMP_EARLYCAM,
	TMM_IPC_TIMESTAMP_RECEIVER,
	TMM_IPC_TIMESTAMP_MAX
}TMM_IPC_TIMESTAMP_TYPE;

typedef enum
{
	TMM_IPC_PUSH_KEY = 0x0001,
	TMM_IPC_ROTARY_KEY = 0x0002,
	TMM_IPC_REQUEST_KEY = 0x0003,	
	TMM_IPC_KEY_MAX
}TMM_IPC_KEY_TYPE;

typedef enum
{
	TMM_IPC_KEY_PRESS = 0x0001,
	TMM_IPC_KEY_RELEASE = 0x0002,
	TMM_IPC_KEY_STATUS_MAX,
}TMM_IPC_KEY_STATUS;

typedef enum
{
	TMM_IPC_HEARTBIT_START = 0x0001,
	TMM_IPC_HEARTBIT_OK,
	TMM_IPC_HEARTBIT_MAX
}TMM_IPC_HEARTBIT_TYPE;

#ifdef HAVE_SDM_DEMO
typedef enum{
	TMM_IPC_SDM_NOTI_NoVideo = 0,
	TMM_IPC_SDM_NOTI_Freeze,
	TMM_IPC_SDM_NOTI_TTError,
	TMM_IPC_SDM_NOTI_NoAudio,
	TMM_IPC_SDM_NOTI_WrongAudio,
	TMM_IPC_SDM_NOTI_UNKNOWN,
	TMM_IPC_SDM_REQ_AUD = 10,
	TMM_IPC_SDM_REQ_DEMO_FV = 20,
	TMM_IPC_SDM_REQ_DEMO_WA,
	TMM_IPC_SDM_REQ_DEMO_NA,
	TMM_IPC_SDM_REQ_DEMO_NV,
	TMM_IPC_SDM_DEMO_IF_MAX
}TMM_IPC_SDM_DEMO_TYPE;

typedef enum{
	TMM_IPC_SDM_DEMO_SND_REQ_TT = 2,
	TMM_IPC_SDM_DEMO_SND_DISABLED = 6,
}TMM_IPC_SDM_DEMO_TYPE_SND;
#endif

typedef enum
{
	TMM_IPC_NVM_WRITE = 0x01,
	TMM_IPC_NVM_READ = 0x02,
	TMM_IPC_NVM_MAX
}TMM_IPC_NVM_TYPE;

typedef enum
{
	TMM_IPC_RVC_ONOFF = 0x01,
	TMM_IPC_RVC_MODE,
	TMM_IPC_RVC_MAX
}TMM_IPC_RVC_TYPE;

typedef enum
{
	TMM_IPC_RESET = 0x01,
	TMM_IPC_RESET_MAX
}TMM_IPC_RESET_TYPE;


#ifndef bool
typedef enum {
	false,
	true
} bool;
#endif

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#endif
