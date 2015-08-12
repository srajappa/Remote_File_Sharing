#include "mnc.h"
#include "error.h"
#include "help.h"

char *systemIP,*systemName;
int systemPort;
struct systemList *top = NULL;
int maxListNum=1;

void startServer(int inputPort){
	
	struct addrinfo hints, *servinfo;

	systemIP = (char*)malloc(sizeof(char)*INET6_ADDRSTRLEN);
		checkAlloc(systemIP);
	systemName = (char*)malloc(sizeof(char)*INET6_ADDRSTRLEN);
		checkAlloc(systemName);
	memset(systemIP,'\0',INET6_ADDRSTRLEN);
	memset(systemName,'\0',INET6_ADDRSTRLEN);

	systemPort = inputPort;
	char str_systemPort[PG];
		memset(str_systemPort,'\0',PG);
	sprintf(str_systemPort,"%d",systemPort);
	

	strcpy(systemIP,findMyIP());
	logEntry("IP Address HOST: ",systemIP,I);

	strcpy(systemName,findMyName());
	logEntry("System Name HOST: ",systemName,I);

	

	//Initialize Linked list to contain value of the central server

	top = addList(top,systemName,systemIP,inputPort,maxListNum++,RESTRICTED);
	log_ret("Details added in List: ",D);


	S_PROMPT;


	int listenFD,maxFD,accFD;
		char cSock[PG];
		memset(cSock,'\0',PG);


	listenFD = Socket(AF_INET,SOCK_STREAM,0);
		sprintf(cSock,"%d",listenFD);


	logEntry("Socket Created: ",cSock,N);

	struct sockaddr_in svrAddr,cliAddr;
	bzero(&svrAddr,sizeof(svrAddr));
	bzero(&cliAddr,sizeof(cliAddr));
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svrAddr.sin_port=htons(systemPort);

	Bind(listenFD,(SA*)&svrAddr, sizeof(svrAddr));
	log_ret("Binding Socket",N);



	Listen(listenFD,LISTENQ);

	maxFD = listenFD;

	fd_set readSet, allSet;
	FD_ZERO(&allSet);
	FD_SET(STDIN,&allSet);					//Registering the standard input file descriptor
	FD_SET(listenFD,&allSet);
	char command[MG],recvMsg[MAX_STR_SIZE];
	int nready,i,j=0,timeout;
	socklen_t addrLen;
	
	struct timeval t;
		t.tv_sec = 1;
		t.tv_usec = 0;
	int numBytes; 

	log_ret("Starting the Select statement",D);

	while(1){
		readSet = allSet;
		timeout = Select(maxFD+1,&readSet,NULL,NULL,NULL);
		//Run through the File descriptors
		//Check for if input is coming, else if accepting a new connection
		//or on time-out sending periodic updates or having received a 
		//content or having a terminated connection.
		for(nready=0; nready <= maxFD ; nready++){
			if(FD_ISSET(nready,&readSet)){

				if(nready == STDIN){
					fgets (command, 30, stdin);
					svrOps(command, commandParser(command),listenFD);
						memset(command,'\0',MG);
					S_PROMPT;

				}else if(nready==listenFD){
					log_ret("New Connection ",N);	//Got a conncetion
					addrLen = sizeof(cliAddr);
					accFD = Accept(listenFD,(SA*)&cliAddr,&addrLen);
					char connAddress[INET6_ADDRSTRLEN];
						memset(connAddress,'\0',INET6_ADDRSTRLEN);

					struct sockaddr_in *sAdd = (struct sockaddr_in *)&cliAddr;
					inet_ntop(AF_INET, &sAdd->sin_addr, connAddress, sizeof connAddress);
					printf("\nConnection request from: %s\n",connAddress);
					logEntry("CONN from: ",connAddress,N);

					S_PROMPT;

					//top = addList(top,,connAddress,6666,maxListNum++,accFD);		//Dummy feed. 
					maxFD = max(maxFD,accFD);
					FD_SET(accFD,&allSet);

				}else{
					
					numBytes = Recv(nready,recvMsg,MAX_STR_SIZE,0);
					if(numBytes==0){
						log_ret("Connection implicitly close",N);
						FD_CLR(nready,&allSet);
						close(nready);
						/*//deleteEntry(nready,top);
						memset(recvMsg,'\0',MAX_STR_SIZE);*/
					}else{
						
						logEntry("Msg from connection ",recvMsg,N);
						connMessageDecode(recvMsg,nready,top);
						memset(recvMsg,'\0',MAX_STR_SIZE);
					}
				}
			}
		}
	}
}

void svrOps(char *command, int decision, int listenFD){
	switch(decision){
		case CREATOR : 	creator();
						break;
		case MYIP:		printf("IP Address: %s\n",systemIP);
						log_ret("CMD: IP Display",I);
						break;
		case MYPORT:	printf("Port: %d\n",systemPort);
						log_ret("CMD: Port Display",I);
						break;
		case HELP: 		svr_help();
						log_ret("CMD: List",I);
						break;
		case LIST: 		displayList(top);
						break;
		case EXIT:		close(listenFD);
						log_ret("CMD: Exit",I);
						Exit(EXIT_APP);
						break;
		case BLANK: 	break;
		
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
