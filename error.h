#ifndef __err_h
#define __err_h

#include "stdarg.h"
#include "time.h"

#define ERR_IO 0
#define ERR_TRANS 1
#define ERR_ABNOR 2
#define EXIT_APP 3
#define EXIT_MASTER 4

#define EXIT_MESSAGE printf("Abnormal Exit: Check LOGS")
#define WRONG_COMMAND printf("Incorrect Command: try HELP\n")
#define CORRECT_REG	printf("Correct Format: \n\t > REGISTER IPADDRESS/(NAME) PORT\n")
#define LIMITED_CONNECTION printf("Not connected to Internet \n \t- Please try again later\n")
/*
void err_ret(char*,...);
void err_sys(char*,...);
void err_dump(char*,...);
void err_msg(char*,...);
void err_quit(char*,...);
static void err_doit(int , int ,char*, va_list);*/

void log_ret(char*,char);
void refresh(int);
void Exit(int);
int validatePort(char *);

#endif