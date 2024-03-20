/****************************************************************************************
 *   FileName    : tc-virtual-key.c
 *   Description : virtucal key c file
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
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <asm/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/signal.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "tc-micom-log.h"
#include "tc-virtual-key.h"

static int	device_fd = -1;

pthread_mutex_t key_mutex = PTHREAD_MUTEX_INITIALIZER;

void die(const char *str) {
	perror(str);
	tmm_virtual_key_close();
	exit(EXIT_FAILURE);
}
static void uinput_ioctl(int32_t fd, uint32_t bit, uint32_t value);
static void post_key(int	fd,uint16_t key, unsigned char status);

static void uinput_ioctl(int32_t fd, uint32_t bit, uint32_t value)
{
	if (ioctl(fd, bit, value) < 0) {
		ERROR_TMM_PRINTF("ioctl fail.\n");
		die("error: ioctl");
	}
}

static void post_key(int	fd,uint16_t key, unsigned char status)
{
	struct input_event     ev_press;
	int32_t ret = -1;

	DEBUG_TMM_PRINTF("key(%d), status(%d) \n", key, status);

	(void)memset(&ev_press, 0, sizeof(ev_press));

	ev_press.type = EV_KEY;
	ev_press.code = key;

	if(status == TMM_KEY_PRESS)
	{
		ev_press.value = 1;
		ret = write(fd, &ev_press, sizeof(ev_press));
		if(ret != sizeof(ev_press))
		{
			ret = -1;
		}
		else
		{
			ret = 0;
		}
	}
	else if(status == TMM_KEY_RELEASE)
	{
		//ev_press.value = 0; /*already zero */
		ret = write(fd, &ev_press, sizeof(ev_press));
		if(ret != sizeof(ev_press))
		{
			ret = -1;
		}
		else
		{
			ret = 0;
		}
	}
	else
	{
		ev_press.value = 1;
		ret = write(fd, &ev_press, sizeof(ev_press));
		if(ret != sizeof(ev_press))
		{
			ret = -1;
		}
		else
		{
			ret = 0;
		}

		if(ret == 0)
		{
			ev_press.value = 0;
			ret = write(fd, &ev_press, sizeof(ev_press));
			if(ret != sizeof(ev_press))
			{
				ret = -1;
			}
			else
			{
				ret = 0;
			}
		}
	}

	if(ret == 0 )
	{
		(void)memset(&ev_press, 0, sizeof(ev_press));
		ev_press.type = EV_SYN;
		ev_press.code = SYN_REPORT;
		(void)write(fd, &ev_press, sizeof(ev_press));
	}
}

void tmm_virtual_key_open(void)
{
	int	fd = -1;

	struct uinput_user_dev uidev;
	TMM_KEY keynum;
	uint32_t key_id;

	fd = open("/dev/input/uinput", O_WRONLY | O_NONBLOCK);
	if ( fd < 0 ) {
		fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	}

	if(fd < 0)
	{
		ERROR_TMM_PRINTF("Can not open uinput.\n");
		die("error: open");
	}

	device_fd = fd;

	uinput_ioctl (fd, UI_SET_EVBIT, EV_KEY);
#if 0
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_LEFT /* KEY_KP4  LEFT */);
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_RIGHT /* KEY_KP6 RIGHT */);
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_UP /* KEY_KP8 UP */);
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_DOWN /* KEY_KP2 DOWN */);
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_ENTER);
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_SPACE);
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_A);
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_B);
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_C);
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_D);
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_E);
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_H);
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_J);
	uinput_ioctl(fd, UI_SET_KEYBIT, KEY_K);
#else
	for(keynum=0;keynum < (TMM_KEY)TMM_KEY_MAX;keynum++){
		key_id = tmm_keymap[keynum].virtualKey;
		if(key_id != KEY_RESERVED)
			uinput_ioctl(fd, UI_SET_KEYBIT,key_id);
	}

	for(keynum=0;keynum<(TMM_KEY)TMM_ROT_MAX;keynum++){
		key_id = tmm_rotarymap[keynum].virtualKey;
		if(key_id != KEY_RESERVED)
			uinput_ioctl(fd, UI_SET_KEYBIT,key_id);
	}

#endif
	(void)memset(&uidev, 0, sizeof(uidev));
	(void)snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "tcc-virtual-kbd");
#if 1
	uidev.id.bustype = BUS_RS232;
	uidev.id.vendor  = 0xDEAD;
	uidev.id.product = 0xBEEF;
	uidev.id.version = 1;
#else
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor  = 0x1;
	uidev.id.product = 0x1;
	uidev.id.version = 1;
#endif
	if(write(fd, &uidev, sizeof(uidev)) < 0)
	{
		ERROR_TMM_PRINTF("uinput write fail.\n");
		die("error: write");
	}

	if(ioctl(fd, UI_DEV_CREATE) < 0)
	{
		ERROR_TMM_PRINTF("Create dev fail(UI_DEV_CREATE).\n");
		die("error: ioctl");
	}

}

void tmm_virtual_key_close(void)
{
	if ( device_fd > 0) {
		ioctl(device_fd, UI_DEV_DESTROY);
		close(device_fd);
		device_fd = -1;
	}

}

void tmm_virtual_key_search(unsigned char key_id, unsigned char status)
{
	int32_t fd = 1;
	TMM_KEY keynum;
	uint32_t send_key;

	for(keynum = 0; keynum<TMM_KEY_MAX; keynum++)
	{
		if(key_id == tmm_keymap[keynum].reciveKey)
		{
			send_key = tmm_keymap[keynum].virtualKey;
			if(send_key != KEY_RESERVED)
			{
				pthread_mutex_lock(&key_mutex);
				fd = device_fd;
				post_key(fd,send_key, status);
				pthread_mutex_unlock(&key_mutex);
			}
		}
	}
}

void tmm_virtual_rotary_search(unsigned char key_id)
{
	int32_t fd = 1;
	TMM_KEY keynum;
	uint32_t send_key;

	for(keynum = 0; keynum<(TMM_KEY)TMM_ROT_MAX; keynum++)
	{
		if(key_id == tmm_rotarymap[keynum].reciveKey)
		{
			send_key = tmm_rotarymap[keynum].virtualKey;
			if(send_key != KEY_RESERVED)
			{
				pthread_mutex_lock(&key_mutex);
				fd = device_fd;
				post_key(fd,send_key, TMM_KEY_SIMPLE);
				pthread_mutex_unlock(&key_mutex);
			}
		}
	}
}

void tmm_vkey_test(void)
{
	int32_t                    fd = -1;
	uint32_t key_id;
	TMM_KEY keynum;

	for(keynum=0;keynum<(TMM_KEY)TMM_KEY_MAX;keynum++){
		key_id = tmm_keymap[keynum].virtualKey;
		if(key_id != 0){
			(void)printf("KEY %d:%d \n",keynum,key_id);
			pthread_mutex_lock(&key_mutex);
			fd=device_fd;
			post_key(fd,key_id, TMM_KEY_SIMPLE);
			pthread_mutex_unlock(&key_mutex);
		}
	}

#if 0
	struct timeval start, end;
	long mtime, seconds, useconds;

	gettimeofday(&start, NULL);
	post_key(fd,KEY_A);
	gettimeofday(&end, NULL);

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	printf("KEY 1 Elapsed time: %ld useconds\n", useconds);

	gettimeofday(&start, NULL);
	post_key(fd,KEY_A);
	post_key(fd,KEY_B);
	gettimeofday(&end, NULL);

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	printf("KEY 2 Elapsed time: %ld useconds\n", useconds);


	gettimeofday(&start, NULL);
	post_key(fd,KEY_A);post_key(fd,KEY_B);post_key(fd,KEY_C);post_key(fd,KEY_D);
	post_key(fd,KEY_E);post_key(fd,KEY_H);post_key(fd,KEY_J);post_key(fd,KEY_K);
	gettimeofday(&end, NULL);

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	printf("KEY 8 Elapsed time: %ld useconds\n", useconds);
#endif
}

void tmm_vkey_test1(void)
{
	int32_t fd = -1;
	fd=device_fd;
	post_key(fd, KEY_H,TMM_KEY_SIMPLE);
	sleep(1);
	post_key(fd, KEY_MEDIA,TMM_KEY_SIMPLE);
	sleep(1);
	post_key(fd, KEY_MEDIA,TMM_KEY_SIMPLE);
	sleep(1);
	post_key(fd, KEY_MEDIA,TMM_KEY_SIMPLE);
	sleep(1);
	post_key(fd, KEY_M,TMM_KEY_SIMPLE);
	sleep(1);
	post_key(fd, KEY_M,TMM_KEY_SIMPLE);
	sleep(1);
	post_key(fd, KEY_EQUAL,TMM_KEY_SIMPLE);
	sleep(1);
	post_key(fd, KEY_MINUS,TMM_KEY_SIMPLE);
	sleep(1);
	post_key(fd, KEY_Z,TMM_KEY_SIMPLE);
	sleep(1);
	post_key(fd, KEY_X,TMM_KEY_SIMPLE);
	sleep(1);
}

