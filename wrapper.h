#ifndef __wrapper_h
#define __wrapper_h

#include "mnc.h"


int Socket(int, int, int);

void Bind(int , const struct sockaddr*, socklen_t );

void Listen(int , int );

int Accept(int , struct sockaddr *, socklen_t *);

void Connect(int , struct sockaddr* , socklen_t );

int Select(int , fd_set *, fd_set *, fd_set *, const struct timeval *);

void Getaddrinfo(char* , char* , struct sockaddr *, struct sockaddr *);

void Getsockname(int, struct sockaddr*, int*);

#endif