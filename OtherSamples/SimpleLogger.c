/*
+--------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------
+Programmer: Nagesh Nanjundachari
+It was my simple logging in C for one of self motivated project.
+This programme will print all log levels based on highest log level.
+That mean,if  you set ERROR, it prints INFO, WARN and ERROR.
+This programme will read nlog.conf file and based on writes the logs to nlog.out file.
+Content of the nlog.conf should be ONLY ONE of the below string as first line.

INFO
WARN
ERROR
DEBUG

After one of above entry in nlog.conf file, run below steps.
./a.out nlog.out &;
kill -s SIGUSR1 <PID of a.out>

Changing log level dynamically....
Change log level entry in the nlog.conf as and when required. Run again below cmd.
kill -s SIGUSR1 <PID of a.out>
---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------
You can extend this programme to module based logging or programme based logging. 
Like this, In same nlog.conf ABC:INFO,  XYZ:DEBUG of a programme. With multipe static global variables in each module. 
And, in case of programmes, different conf files each programme.
+--------------------------------------------------------------------------------------------------------------------
*/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

#define D_INF 0x01
#define D_WAR 0x02
#define D_ERR 0x04
#define D_DBG 0x08

#define LVL_INF 0x01
#define LVL_WAR 0x03
#define LVL_ERR 0x07
#define LVL_DBG 0x0F

#define _LINF_ (nncode & D_INF)
#define _LWAR_ (nncode & D_WAR)
#define _LERR_ (nncode & D_ERR)
#define _LDBG_ (nncode & D_DBG)

#define log_info  if(_LINF_)fprintf
#define log_warn  if(_LWAR_)fprintf
#define log_error if(_LERR_)fprintf
#define log_debug if(_LDBG_)fprintf

#define LSET 1
#define MAX_BUF 10

int nncode = 0;
char level[MAX_BUF];
static struct sigaction sa;
FILE *lfp;

static void log_handler(int sig_num) {

FILE *fp = NULL;
int lset = 0;

	fp = fopen("nlog.conf", "r");
        if (fp) {
		fgets(level, MAX_BUF, fp);
		level[strlen(level)-1]=0;

		if(!strcmp(level, "INFO" )) { lset=LSET; nncode=LVL_INF; }
		if(!strcmp(level, "WARN" )) { lset=LSET; nncode=LVL_WAR; }
		if(!strcmp(level, "ERROR")) { lset=LSET; nncode=LVL_ERR; }
		if(!strcmp(level, "DEBUG")) { lset=LSET; nncode=LVL_DBG; }
	}
        if (!(fp) || !lset) {
		printf ("Pls chk nlog.conf file. Not exist OR content is incorrect\n\r");
	}
	if (fp) fclose(fp);
}

int main (int argc, char*argv[]) {

    if (argc != 2) { printf ("Usage: ./a.out nlog.out &\n\r"); exit (0); }
    if (strcmp ("nlog.out", argv[1])) { printf ("Usage: ./a.out nlog.out &\n\r"); exit (0); }

    printf ("If nlog.conf exist, now run: kill -s SIGUSR1 <pid-of-a.out>\n\r");

    lfp = fopen("nlog.out", "ab");

    sa.sa_handler = log_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; 
    sigaction(SIGUSR1, &sa, NULL);

	while (1) {
		// Eg., This programme logs for every 15 sec.
		sleep (15);
		log_info(lfp, "INFO message printed.\n");
		log_warn(lfp, "INFO ++ WARN message printed.\n");
		log_error(lfp,"INFO ++ WARN ++ ERROR message printed.\n");
		log_debug(lfp,"INFO ++ WARN ++ ERROR ++ DEBUG message printed.\n");
		fflush(lfp);
	}
}
