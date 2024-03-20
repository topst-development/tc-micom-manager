/****************************************************************************************
 *   FileName    : tc-micom-manager.h
 *   Description : Micom Manager Header File
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
#ifndef TC_MICOM_MANAGER_H
#define TC_MICOM_MANAGER_H

void * micom_ipc_handle;
void * ap_ipc_handle;

typedef struct
{
	int32_t daemonize;
	int32_t virtualKeyTest;
} TMM_Flags;

#define TMM_DAEMON_UMASK              027

typedef enum _ipc_request_key_type{
	eRequest_Key_ALL=1,
	eRequest_Key_IVI,
	eRequest_Key_SVM,
}ipc_request_key_type;

int32_t tmm_initialize(const char * micom_device, const char * ap_device,ipc_request_key_type host_type);
void tmm_release(void);
int32_t tmm_resume(const char * micom_device, const char * ap_device,ipc_request_key_type host_type);
void tmm_suspend(void);


#endif /* TC_MICOM_MANAGER_H */
