#include "mnc.h"
#include "error.h"
#include "help.h"

char *systemIP,*systemName;
int systemPort;
struct systemList *top;
int maxListNum;

void startServer(int inputPort){
	
	systemIP = (char*)malloc(sizeof(char)*INET6_ADDRSTRLEN);
		checkAlloc(systemIP);
	systemName = (char*)malloc(sizeof(char)*INET6_ADDRSTRLEN);
		checkAlloc(systemName);
	memset(systemIP,'\0',INET6_ADDRSTRLEN);
	memset(systemName,'\0',INET6_ADDRSTRLEN);

	systemPort = inputPort;

	log_ret("Calculating the IP address",I);
	strcpy(systemIP,findMyIP());

	log_ret("Finding the host name of system",I);
	strcpy(systemName,findMyName());

	log_ret("Entry in List: ",D);
		top = (struct systemList*)malloc(sizeof(struct systemList));
		checkAlloc(top);
	SystemList(top);
	maxListNum =1;
	addToList(top,systemName,systemIP,inputPort,maxListNum++,RESTRICTED);
	


	S_PROMPT;

	log_ret("Preparing the socket then binding it",N);

	int listenFD,maxFD,accFD;

	listenFD = Socket(AF_INET,SOCK_STREAM,0);


	struct sockaddr_in svrAddr,cliAddr;
	bzero(&svrAddr,sizeof(svrAddr));
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svrAddr.sin_port=htons(SERV_PORT);

	Bind(listenFD,(SA*)&svrAddr,sizeof(svrAddr));

	Listen(listenFD,LISTENQ);

	maxFD = listenFD;

	fd_set readSet, allSet;
	FD_ZERO(&allSet);
	FD_SET(STDIN,&allSet);					//Registering the standard input file descriptor
	FD_SET(listenFD,&allSet);
	char command[MG];
	int nready,i,j=0;
	socklen_t addrLen;
	
	struct timeval t;
		t.tv_sec = 1;
		t.tv_usec = 0;


	log_ret("Starting the Select statement",D);

	while(1){
		readSet = allSet;
		nready = Select(maxFD+1,&readSet,NULL,NULL,&t);
		/*If Standard input*/
		if(FD_ISSET(STDIN, &readSet)){
			fgets (command, 30, stdin);
			commandOutput(commandParser(command));
				memset(command,'\0',MG);
			S_PROMPT;
		}else if(FD_ISSET(listenFD,&readSet)){
			log_ret("Received an accept ",N);	//Got a conncetion
			addrLen = sizeof(cliAddr);
			accFD = Accept(listenFD,(SA*)&cliAddr,sizeof(cliAddr));
			extractInfoAndAdd(cliAddr,top,maxListNum,accFD);
			sendUpdate(accFD,top);
			maxFD = max(maxFD,accFD);
			FD_SET(accFD,&allSet);
		}else{	//time rotation
			log_ret("Periodic Update to all nodes",N);
			periodicUpdate(top);
		}
		/*Sending list to the one's who just connected*/
		for(i=1;i<=maxFD;i++){
			if(i!=listenFD){
				if(FD_ISSET(i,&readSet)){
					
				}
			}
		}
		/*Periodic update is sent here below*/
	}
}

void commandOutput(int decision){
	switch(decision){
		case CREATOR : 	creator();
						break;
		case MYIP:		printf("%s\n",systemIP);
						log_ret("CMD: IP Display",I);
						break;
		case MYPORT:	printf("%d\n",systemPort);
						log_ret("CMD: Port Display",I);
						break;
		case HELP: 		svr_help();
						break;
		case LIST: 		displayList(top);
						break;
		case EXIT:		Exit(EXIT_APP);
		default:		WRONG_COMMAND;
	}
}


/*CREATOR		1
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
#define STATISTICS  12*/
