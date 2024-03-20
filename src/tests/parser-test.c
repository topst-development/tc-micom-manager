/****************************************************************************************
 *   FileName    : parser-test.c
 *   Description : parser test
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited
to re-distribution in source or binary form is strictly prohibited.
This source code is provided “AS IS” and nothing contained in this source code
shall constitute any express or implied warranty of any kind, including without limitation,
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent,
copyright or other third party intellectual property right.
No warranty is made, express or implied, regarding the information’s accuracy,
completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from,
out of or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "tc-parser.h"
#include "tc-crc.h"

static void SendDataPrint(TMM_IPC_Protocol *data)
{
	int32_t i;
	(void)fprintf(stderr, "CallBack Function Called\n");
	(void)fprintf(stderr, "Command 1 : 0x%x\n", data->cmd1);
	(void)fprintf(stderr, "Length : %x\n", data->length);
	(void)fprintf(stderr, "Command 2 : 0x%x\n", data->cmd2);
	for(i = 0; i < data->length; i++)
	{
		(void)fprintf(stderr, "Data : %x\n", *data->data);
		data->data++;
	}
}

void main()
{
	int32_t i;

	(void)fprintf(stderr, "hello parser\n");
	uint32_t bufs_size = 17;
	unsigned char bufs[17] =  {0xff, 0x55, 0xcc, 0xff, 0x55, 0xaa, 0x01, 0x02, 0x0a, 0xb1, 0x00, 0x03, 0xff, 0x55, 0xaa};
	uint16_t crc = tmm_CRC16(&bufs[3], 12, 0);

	bufs[15] = (char)((crc & 0xff00) >> 8);
	bufs[16] = (char)(crc & 0x00ff);
	(void)fprintf(stderr, "ori_buf:");
	for(i = 0; i < bufs_size; i++)
	{
		fprintf(stderr, "0x%x ", bufs[i]);
	}
	(void)fprintf(stderr, "\n");
	//tmm_parser_decoder(&bufs[0], 17);
	//tmm_parser_decoder(&bufs[0], 8);
	//tmm_parser_decoder(&bufs[8], 9);
}
