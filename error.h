#ifndef __err_h
#define __err_h

void err_ret(char*,...);
void err_sys(char*,...);
void err_dump(char*,...);
void err_msg(char*,...);
void err_quit(char*,...);
static void err_doit(int , int ,char*, va_list);

#endif