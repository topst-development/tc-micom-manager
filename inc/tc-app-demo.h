/****************************************************************************************
 *   FileName    : tc-app-demo.h
 *   Description : applicaiton demo File
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
#ifndef TC_DEMO_APP_H
#define TC_DEMO_APP_H

#define RVC_ON		  0x1
#define RVC_OFF		  0x2

#define RESET_START		0x01

/* Call from dbus message*/
void tmm_rvc_on_method_received(void);
void tmm_rvc_off_method_received(void);
void tmm_reset_method_received(void);
void tmm_robust_method_received(void);
void tmm_change_camera_mode_method_received(void);


/* Call from ipc */
void tmm_rvc_ipc_received(uint16_t cmd, unsigned char *buf, uint32_t length);
void tmm_reset_ipc_received(uint16_t cmd);
void tmm_panic_ipc_received(void);
void tmm_change_camera_mode_ipc_received(void);


#endif
