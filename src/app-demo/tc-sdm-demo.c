/****************************************************************************************
 *   FileName    : tc-sdm-demo.c
 *   Description : sdm demo c file
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

#include "tc-micom-log.h"
#include "tc-micom-protocol.h"
#include "tc-micom-manager.h"

void tmm_sdm_ipc_received(uint16_t cmd, unsigned char *buf, uint32_t length)
{
	static int audio_demo_flag = TMM_IPC_SDM_REQ_AUD;
    static int sdm_aud_msg_preset = 0;
	static int old_audio_demo_flag = 0;
	static int old_data = -1;

    switch(cmd)
    {
        case TMM_IPC_SDM_NOTI_NoVideo:
            DEBUG_TMM_PRINTF("SDM NOTI: TMM_IPC_SDM_NOTI_NoVideo x%x, x%x\n", buf[0], buf[1]);
            break;
        case TMM_IPC_SDM_NOTI_Freeze:
            DEBUG_TMM_PRINTF("SDM NOTI: TMM_IPC_SDM_NOTI_Freeze x%x, x%x\n", buf[0], buf[1]);
            break;
        case TMM_IPC_SDM_NOTI_TTError:
            DEBUG_TMM_PRINTF("SDM NOTI: TMM_IPC_SDM_NOTI_TTError x%x, x%x\n", buf[0], buf[1]);
            break;
        case TMM_IPC_SDM_NOTI_NoAudio:
            DEBUG_TMM_PRINTF("SDM NOTI: TMM_IPC_SDM_NOTI_NoAudio x%x, x%x\n", buf[0], buf[1]);
            break;
        case TMM_IPC_SDM_NOTI_WrongAudio:
            DEBUG_TMM_PRINTF("SDM NOTI: TMM_IPC_SDM_NOTI_WrongAudio x%x, x%x\n", buf[0], buf[1]);
            break;
        case TMM_IPC_SDM_NOTI_UNKNOWN:
            DEBUG_TMM_PRINTF("SDM NOTI: TMM_IPC_SDM_NOTI_UNKNOWN x%x, x%x\n", buf[0], buf[1]);
            break;
        case TMM_IPC_SDM_REQ_AUD:
            INFO_TMM_PRINTF("SDM REQ: TMM_IPC_SDM_REQ_AUD x%x, x%x\n", buf[0], buf[1]);
            #if 1
            if(sdm_aud_msg_preset == 0)
            {
                system("amixer -c 0 sset 'Device0 Clock Continuous Mode',0 off");
                sdm_aud_msg_preset = 1;
            }
            else
            #endif
            {
                if((old_data == buf[0]))
                {
                    ERROR_TMM_PRINTF("Warning!!! It's a same command. Does nothing. flag x%x, idx x%x\n", old_audio_demo_flag, old_data);
                    break;
                }

                system("killall aplay");
                old_audio_demo_flag = audio_demo_flag;
                old_data = buf[0];
                if(buf[0])
                {
                    if(audio_demo_flag == TMM_IPC_SDM_REQ_AUD)
                    {
                        system("/usr/bin/aplay /media/clk48k_16bit_RJ.wav &");
                    }
                    else if(audio_demo_flag == TMM_IPC_SDM_REQ_DEMO_WA)
                    {
                        system("/usr/bin/aplay /media/broken.wav &");
                    }
                    else if(audio_demo_flag == TMM_IPC_SDM_REQ_DEMO_NA)
                    {
                        system("/usr/bin/aplay /media/clk48k_16bit_RJ.wav &");
                        sleep(5);
                        system("killall aplay");
                    }
                    else
                    {
                        ERROR_TMM_PRINTF("Unknown audio flag....x%x\n", audio_demo_flag);
                    }
                    audio_demo_flag = TMM_IPC_SDM_REQ_AUD;
                }
            }
            break;
        case TMM_IPC_SDM_REQ_DEMO_WA:
            DEBUG_TMM_PRINTF("SDM REQ: TMM_IPC_SDM_REQ_DEMO_WA x%x, x%x\n", buf[0], buf[1]);
            audio_demo_flag = TMM_IPC_SDM_REQ_DEMO_WA;
            break;
        case TMM_IPC_SDM_REQ_DEMO_NA:
            DEBUG_TMM_PRINTF("SDM REQ: TMM_IPC_SDM_REQ_DEMO_NA x%x, x%x\n", buf[0], buf[1]);
            audio_demo_flag = TMM_IPC_SDM_REQ_DEMO_NA;
            break;
        case TMM_IPC_SDM_REQ_DEMO_FV:
            DEBUG_TMM_PRINTF("SDM REQ: TMM_IPC_SDM_REQ_DEMO_FV x%x, x%x\n", buf[0], buf[1]);
            if(buf[1])
            {
                DEBUG_TMM_PRINTF("KILL CLUSTER\n");
                system("/etc/init.d/qt-cluster stop");
            }
            else
            {
                DEBUG_TMM_PRINTF("START CLUSTER FV\n");
                system("/etc/init.d/qt-cluster start normal");

            }

            break;
        case TMM_IPC_SDM_REQ_DEMO_NV:
            DEBUG_TMM_PRINTF("SDM REQ: TMM_IPC_SDM_REQ_DEMO_NV x%x, x%x\n", buf[0], buf[1]);
            if(buf[1])
            {
                DEBUG_TMM_PRINTF("KILL CLUSTER\n");
                system("/etc/init.d/qt-cluster stop");
            }
            else
            {
                DEBUG_TMM_PRINTF("START CLUSTER NV\n");
                system("/etc/init.d/qt-cluster start");
            }

            break;
        default:
            DEBUG_TMM_PRINTF("SDM UNKNOWN: x%x, x%x, x%x\n", cmd, buf[0], buf[1]);
            break;
    }
}


