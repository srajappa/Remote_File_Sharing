/*
 * FileName: error.c
 * Author: Srinivasan Rajappa
 * Created: 05 June, 2015
 * Description: This C file contains functions that aid to log and print error statements in scenarios where
 * functions tend to return system related errors. 
 * ----REFERENCES-----
 * The motivation for creating this file was obtained after going through the material in the book
 * Unix Networking Programming (written by W. Richard Stevens). The author has created a more exhaustive
 * header file called <error.c> which can be found in Appendix D of the second edition book.
 */


#include	"mnc.h"
#include 	"help.h"
#include 	"error.h"
#include	<stdarg.h>
#include	<syslog.h>



int		daemon_proc;

static void err_doit(int, int, const char*, va_list);

char* systemDate(){
   	time_t now = time(NULL);
   	struct tm *t = localtime(&now);
   	float milliSec = t->tm_sec;
   	milliSec/=1000;
   	char *stringDate = (char*)malloc(sizeof(char)*MG);
   		memset(stringDate,'\0',MG);
   	snprintf(stringDate,MG,"<%d-%d-%d:%d-%d-%d-%-0.3f>",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,milliSec);
   	return stringDate;
}


void log_ret(char *str,char mode){
	FILE *writeFD;
	writeFD = fopen(FILENAME,"a");
	fprintf(writeFD, "%-20s[%c]->%-50s\n",systemDate(),mode,str);
	fflush(writeFD);
	close(writeFD);
}

void refresh(int MODE){
	FILE *writeFD;
	writeFD = fopen(FILENAME,"a");
	if(MODE==START){
		fprintf(writeFD, "\n\nft_main Starts [%s]==================================================\n",systemDate() );
	}else{
		fprintf(writeFD, "ft_main Stops  [%s]**************************************************\n",systemDate() );
	}
	fflush(writeFD);
	close(writeFD);
}

void Exit(int i){
	FILE *writeFD;
	writeFD = fopen(FILENAME,"a");
	switch(i){
		case ERR_IO : 	fprintf(writeFD, "EXIT: I/O Error\n");
					  	break;
		case ERR_TRANS: fprintf(writeFD, "EXIT: System Error \n");
						break;
		case EXIT_APP:	fprintf(writeFD, "EXIT: User exited from application\n");
						break;
		default:      	fprintf(writeFD, "Exit----> argument is incorrect at <error.c>\n");
	}
	fprintf(writeFD, "ft_main Stops [%s]**************************************************\n",systemDate() );
	fflush(writeFD);
	close(writeFD);
	exit(i);
}
/* Nonfatal error related to a system call.
 * Print a message and return. 
 */

void err_ret(const char*fmt,...){
	va_list	ap;

	va_start(ap,fmt);
	err_doit(1,LOG_INFO,fmt,ap);
	va_end(ap);
	return;
}

/* Fatal error related to a system call.
 * Print a messsage and terminate.
 */

void err_sys(const char *fmt,...){
	va_list ap;

	va_start(ap,fmt);
	err_doit(1,LOG_ERR,fmt,ap);
	va_end(ap);
	exit(1);
}

/* Fatal error related to a system call.
 * Print a message, dump core and terminate
 */

void err_dump(const char *fmt,...){
	va_list ap;

	va_start(ap,fmt);
	err_doit(1,LOG_ERR,fmt,ap);
	va_end(ap);
	abort();		//Dumping core and terminate
	exit(1);
}

/* Nonfatal error unrelated to a system call
 * Print a message and return
 */

void err_msg(const char *fmt,...){
	va_list ap;

	va_start(ap,fmt);
	err_doit(1,LOG_ERR,fmt,ap);
	va_end(ap);
	return;
}

/* Fatal error unrelated to a system call
 * Print a message and return
 */

void err_quit(const char *fmt,...){
	va_list ap;

	va_start(ap,fmt);
	err_doit(1,LOG_ERR,fmt,ap);
	va_end(ap);
	exit(1);
}

/* Print a message and return to caller.
 * Caller specifies "errnoflag" and "level".
 */

static void err_doit(int errnoflag, int level, const char *fmt, va_list ap){
	int errno_save, n;
	char buf[MAXLINE];

	errno_save = errno;

	/*#ifdef HAVE_VSNPRINTF
		vsnprintf(buf,sizeof(buf),fmt,ap);
	#else
		vsnprintf(buf,fmt,ap);
	#endif*/

	n = strlen(buf);
	if(errnoflag)
		snprintf(buf+n,sizeof(buf)-n,": %s",strerror(errno_save));
	strcat(buf,"\n");

	if(daemon_proc){
		syslog(level,buf);
	} else{
		fflush(stdout);
		fputs(buf,stderr);
		fflush(stderr);
	}
	return;
}