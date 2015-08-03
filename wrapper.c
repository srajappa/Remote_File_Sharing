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
		log_net_error("Socket Error",E,n);
	}
	return(n);
}

/*bind function*/
void Bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen){
	int val;
	if((val = bind(sockfd,myaddr,addrlen)) < 0){
		log_net_error("Bind Error",E,val);
		printf("\nBind Error\n");
		Exit(ERR_TRANS);
	}
}

/*listen function*/
void Listen(int fd, int backlog){
	int val;
	if((val=listen(fd,backlog))<0){
		log_net_error("Listen Error",E,val);
		printf("\nListen Error\n");
		Exit(ERR_TRANS);
	}
}

/*accept function*/
int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen){
	int val;
	if((val = accept(sockfd,cliaddr,addrlen))<0){
		if(val==EBADF){
			log_net_error("Accept: Descriptor Invalid",N,val);	
		}else if(val==EINVAL){
			log_net_error("Accept: Socket no conn [addrlen not valid]",N,val);
		}else if(val==EPROTO){
			log_net_error("Accept: Protocol Error",N,val);
		}else if(val==EPERM){
			log_net_error("Accept: Firewall forbids",N,val);
		}else{
			log_net_error("Accept Error",N,val);
			printf("\nAccept Error\n" );
		}
		//printf("\n\n");
	}
	return(val);
}

/*connect function*/

void Connect(int fd, struct sockaddr* servaddr, socklen_t addrlen){						
	int val;
	if((val = connect(fd,servaddr,addrlen))<0){
		
		log_net_error("Connect Error",N,val);

		printf("\nUnable to Connect\n");
	}
	//return val;
}




/*select function*/
int Select(int maxfd, fd_set *readset, fd_set *writeset, fd_set *exceptionset, const struct timeval *timeout){
	int val;
	if((val=select(maxfd,readset,writeset,exceptionset,timeout)) < 0){
		log_net_error("Select Error",E,val);
	}
	return(val);
}

/* getaddrinfo function */
void Getaddrinfo(char IP[], char port[], struct sockaddr *servaddr, struct sockaddr *target){
	int val;
	if((val=getaddrinfo(IP,port,servaddr,target))<0){
		log_net_error("Getaddrinfo Error",N,val);
	}
	//return(val);
}

/* getsockname function */
void Getsockname(int sockFD, struct sockaddr *servaddr, int *length){
	int val;
	if((val=getsockname(sockFD,servaddr,length))<0){
		log_net_error("Getaddrinfo Error",N,val);
	}
	//return(val);
}

void Send(int sockFD,const void *msg,int len,int flags){
	int val;
	if((val=send(sockFD,msg,len,flags))<0){
		log_net_error("Sending Failed",N,val);
	}
}

int Recv(int sockFD,const void *msg, int len, int flags){
	int val;
	if((val=recv(sockFD,msg,len,flags))<0){
		log_net_error("Recv Failed",N,val);
	}
	return val;
}
