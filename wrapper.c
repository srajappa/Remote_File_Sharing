/*
 * FileName: wrapper.c
 * Author: Srinivasan Rajappa
 * Created: 19 June, 2015
 * Description: This C file contains the alias for functions. It is created in order to 
 * incorporate error messages and to improve code reusability. 
 * ----REFERENCES-----
 * The motivation for creating this file was obtained after going through the material in the book
 * Unix Networking Programming (written by W. Richard Stevens). 
 */

 #include "mnc.h"
 #include "wrapper.h"
 #include "error.h"
 #include "help.h"

/*socket function*/
int Socket(int family, int type, int protocol){
	int n;
	if((n=socket(family,type,protocol))<0){
		log_ret("Socket Error",E);
	}
	return(n);
}

/*bind function*/
void Bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen){
	int val;
	if((val = bind(sockfd,myaddr,addrlen)) < 0){
		log_ret("Bind Error",E);
	}
}

/*listen function*/
void Listen(int fd, int backlog){
	char *ptr;
	/*	if((ptr=getenv("LISTENQ"))!=NULL){
		backlog = atoi(ptr);
	}*/								

	if(listen(fd,backlog)<0)
		log_ret("Listen Error",E);
}

/*accept function*/
int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen){
	int val;
	if((val = accept(sockfd,cliaddr,addrlen))<0){
		log_ret("Accept Error",N);
	}
	return(val);
}

/*connect function*/
void Connect(int fd, struct sockaddr* servaddr, socklen_t addrlen){						
	if(connect(fd,servaddr,addrlen)<0)
		log_ret("Connect Error",N);
}

/*select function*/
int Select(int maxfd, fd_set *readset, fd_set *writeset, fd_set *exceptionset, const struct timeval *timeout){
	int val;
	if((val=select(maxfd,readset,writeset,exceptionset,timeout)) < 0){
		log_ret("Select Error",E);
	}
	return(val);
}

/* getaddrinfo function */
void Getaddrinfo(char IP[], char port[], struct sockaddr *servaddr, struct sockaddr *target){
	int val;
	if((val=getaddrinfo(IP,port,servaddr,target))<0){
		log_ret("Getaddrinfo Error",N);
	}
	//return(val);
}

/* getsockname function */
void Getsockname(int sockFD, struct sockaddr *servaddr, int *length){
	int val;
	if((val=getsockname(sockFD,servaddr,length))<0){
		log_ret("Getaddrinfo Error",N);
	}
	//return(val);
}

void Send(int sockFD,const void *msg,int len,int flags){
	int val;
	if((val=send(sockFD,msg,len,flags))<0){
		log_ret("Sending Failed",N);
	}
}