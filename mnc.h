/*
 * FileName: mnc.h
 * Author: Srinivasan Rajappa
 * Created: 05 June, 2015
 * Description: The header file mnc is created to contain all the headers and also contains custom 
 * defined constants and macros. The file is organized as follows: 
 * a) Header Files
 * b) checking constraints of header files
 * c) defining constants and macros
 * ----REFERENCES-----
 * The motivation for creating this file was obtained after going through the material in the book
 * Unix Networking Programming (written by W. Richard Stevens). The author has created a more exhaustive
 * header file called <unp.h> which can be found in Appendix D of the second edition book.
 * Edited: 18 June, 2015 (Git Branch: revelation1)
 */

#ifndef __mnc_h
#define __mnc_h

#include<sys/types.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<fcntl.h>
#include<netdb.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/uio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/un.h>

#ifdef 	HAVE_SYS_SELECT_H
#include<sys/select.h>
#endif

#ifdef 	HAVE_POLL_H
#include<poll.h>
#endif

#ifdef 	HAVE_STRINGS_H
#include<strings.h>
#endif

#ifdef HAVE_SYS_IOCTL_H
#include<sys/ioctl.h>
#endif
#ifdef HAVE_SYS_FILIO_H
#include<sys/filio.h>
#endif
#ifdef HAVE_SYS_SOCKIO_H
#include<sys/sockio.h>
#endif

#ifdef HAVE_PTHREAD_H
#include<pthread.h>
#endif

/*
 * Defining some custom constants and macros
 */

#define MAXSIZE			64
#define LISTENQ			1024
#define FD_SETSIZE		10
#define MAXLINE			1024
#define	MAXSOCKADDR		128
#define	BUFFSIZE		10000
#define STDIN			0
#define	SERV_PORT		9877
#define	SERV_PORT_STR 	"9877"  
#define SA 				struct sockaddr
#define min(a,b)		((a)<(b) ? (a):(b))
#define	max(a,b)		((a)>(b) ? (a):(b))

#define SERVER_MODE 0
#define CLIENT_MODE 1
#define RESTRICTED -999

#define S_PROMPT printf("Svr@[PA-1]> "); fflush(stdout)
#define C_PROMPT printf("Cli@[PA-1]> "); fflush(stdout)



#define CREATOR		1
#define HELP    	2
#define MYIP    	3
#define MYPORT    	4
#define REGISTER    5 
#define CONNECT    	6
#define LIST    	7
#define TERMINATE   8
#define EXIT    	9
#define UPLOAD    	10
#define DOWNLOAD    11
#define STATISTICS  12

#endif