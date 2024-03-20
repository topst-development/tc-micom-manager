/****************************************************************************************
 *   FileName    : main.c
 *   Description : main c file
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
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/kd.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <linux/types.h>
#include <linux/fb.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>
#include <termios.h>
#include <ctype.h>

#include "tc-micom-log.h"
#include "tc-micom-manager.h"

int32_t g_tmm_debug = 0;
ipc_request_key_type key_type = eRequest_Key_ALL;



TMM_Flags tmmFlags;
const char *_pid_file = "/var/run/tc-micom-manager.pid";

#define DEFAULT_DEVICE_NAME "/dev/tcc_ipc_micom"
const char *device_name = NULL;
const char *ap_device_name = NULL;


static struct option long_opt[] = {
	{"device", required_argument, 0, 'D'},
	{"daemon", required_argument, 0, 'd'},
	{"log", required_argument, 0, 'l'},
	{"key", required_argument, 0, 'k'},
	{"device_ap", required_argument, 0, 'A'},
	{"help", no_argument, 0, 'h'},
	{0, 0, 0, 0}
};

volatile int32_t ipcExit;

static void help_msg(char* argv)
{
	(void)printf("------------------------------------------------------------\n"
           "|  Linux_IVI tc-micom_manager help\n"
           "| ===========================\n"
           "| <Daemonize running>\n"
           "|    ex) daemon on \n"
           "|       $ --daemon=1 or -d 1 \n"
           "|    ex) daemon off \n"
           "|       $ --daemon=0 or -d 0 \n"
           "| <Select device for micom>\n"
           "|    ex) select ipc for micom device \n"
           "|       $ --device=/dev/tcc_ipc_micom -D /dev/tcc_ipc_micom \n"
           "|	Default device is /dev/tcc_ipc_micom\n"
		   "| <Select device for AP>\n"
		   "|	  ex) select ipc for AP device \n"
		   "|		 $ --device_ap=/dev/tcc_ipc_ap -A /dev/tcc_ipc_ap \n"
           "| <Debug log>\n"
           "|    ex) set debug level \n"
           "|       $ --log=<debug level> or -l <debug level> \n"
           "|     error   : 0 \n"
           "|     warning : 1 \n"
           "|     info    : 2 \n"
           "|     debug   : 3 \n"
           "| <select host>\n"
           "|    ex) select request key: all or ivi or svm\n"
           "|       $ --key=all -k all \n"
           "|       $ --key=sub_ivi -h sub_ivi \n"
           "|       $ --key=svm -h svm \n"
           "|	Default is all \n"
           "|\n"
           "------------------------------------------------------------\n");
}

static void signal_handler(int32_t sig)
{
	INFO_TMM_PRINTF("Get Signal %d\n", sig);
    if(sig == SIGTSTP)
    {
        INFO_TMM_PRINTF("SIGTSTP\n");
		tmm_suspend();
        raise(SIGSTOP);
    }
    else if(sig == SIGCONT)
    {
        INFO_TMM_PRINTF("SIGCONT\n");
		tmm_resume(device_name, ap_device_name, key_type);
		INFO_TMM_PRINTF("SIGCONT done\n");
    }
    else
    {
		ipcExit =0;
		tmm_release();
		exit(sig);
    }

}

static void daemonize(void)
{
	pid_t pid = 0;
	FILE *pid_fp;

	// create child process
	pid = fork();

	// fork failed
	if (pid < 0)
	{
		ERROR_TMM_PRINTF("fork failed\n");
		exit(1);
	}
	// parent process
	if (pid > 0)
	{
		// exit parent process for daemonize
		exit(0);
	}
	// umask the file mode
	umask(0);

	// create pid file

	pid_fp = fopen(_pid_file, "w+");
	if (pid_fp != NULL)
	{
		(void)fprintf(pid_fp, "%d\n", getpid());
		fclose(pid_fp);
		pid_fp = NULL;
	}
	else
	{
		ERROR_TMM_PRINTF("pid file open failed");
	}
	// to do: open log

	// set new session
	if (setsid() < 0)
	{
		ERROR_TMM_PRINTF("set new session failed\n");
		exit(1);
	}

	// change the current working directory for safety
	if (chdir("/") < 0)
	{
		ERROR_TMM_PRINTF("change directory failed\n");
		exit(1);
	}
}

int32_t main(int32_t argc, char* argv[])
{
	char switch_num;
	int32_t option_idx = 0;
	int32_t opt_val = 0;
	int32_t ret = 0;

	ipcExit =1;

	(void)memset(&tmmFlags,0,sizeof(TMM_Flags));

	if (argc < 2) {
		help_msg(argv[0]);
		ret = -1;
	}
	else {

		DEBUG_TMM_PRINTF("program start\n");
		while(1)
		{
			switch_num = getopt_long(argc, argv,"D:d:l:k:A:h" , long_opt, &option_idx);
			if (switch_num == -1) { break; }
			if (switch_num == 255) { break; }
			if(optarg) opt_val = atoi(optarg);

			switch(switch_num)
			{
				case 0:
					if (long_opt[option_idx].flag != 0)
						break;

					INFO_TMM_PRINTF("option %s", long_opt[option_idx].name);
	
					if (optarg)
						INFO_TMM_PRINTF(" with arg %s\n", optarg);
				break;
				
				case 'D':
					device_name = optarg;
					INFO_TMM_PRINTF("set ipc name : %s \n", device_name);
				break;

				case 'A':
					ap_device_name = optarg;
					INFO_TMM_PRINTF("set sub ipc name : %s \n", ap_device_name);
				break;

				case 'd':
					if(opt_val){
						tmmFlags.daemonize = opt_val;
					}
					else{
						tmmFlags.daemonize = 0;
					}
					INFO_TMM_PRINTF("Daemonize: 0x%x \n", opt_val);
				break;

				case 'l':
					if(opt_val){
						g_tmm_debug = opt_val;
					}
					else{
						g_tmm_debug= 0;
					}
					INFO_TMM_PRINTF("debug log : 0x%x \n", g_tmm_debug);
				break;

				case 'k':
					if(optarg != NULL)
					{
						INFO_TMM_PRINTF("key type : %s \n", optarg);
						if(strncmp(optarg,"ivi",3)==0)
						{
							key_type = eRequest_Key_IVI;
						}
						else if(strncmp(optarg,"svm",3)==0)
						{
							key_type = eRequest_Key_SVM;
						}
						else
						{
							key_type = eRequest_Key_ALL;
						}
					}
				break;

				case 'h':
					help_msg(NULL);
				break;

				default :
					INFO_TMM_PRINTF("Wrong options %d \n",switch_num);
				break;
			}
		}

		if (tmmFlags.daemonize )	{
			daemonize();
		}

		signal(SIGINT, signal_handler);
		signal(SIGTERM, signal_handler);
		signal(SIGABRT, signal_handler);
		signal(SIGKILL, signal_handler);
		signal(SIGTSTP, signal_handler);
		signal(SIGCONT, signal_handler);

		ret = tmm_initialize(device_name, ap_device_name, key_type);
		if(ret > -1)
		{
			while(ipcExit)
			{
				sleep(1);
			}
		}
		else
		{
			ERROR_TMM_PRINTF("Can not initialize\n");
		}

		tmm_release();

	}
	if (access(_pid_file, F_OK) == 0)
	{
		if (unlink(_pid_file) != 0)
		{
			ERROR_TMM_PRINTF("delete pid file failed\n");
		}
	}

	return ret;
}


