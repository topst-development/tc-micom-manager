/****************************************************************************************
 *   FileName    : tc-micom-dbusmanager.h
 *   Description : DBus manager Header file
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
#ifndef TMM_MICOM_DBUSMANAGER_H
#define TMM_MICOM_DBUSMANAGER_H

void tmm_dbus_initialize(void);
void tmm_dbus_release(void);

void tmm_dbus_emit_signal(uint32_t name);
void tmm_dbus_emit_signal_data(uint32_t name, int32_t data);
void tmm_dbus_emit_signal_char(uint32_t name, char data);

#endif
