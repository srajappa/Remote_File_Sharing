#include "mnc.h"
#include "error.h"
#include "help.h"

char *systemIP,*systemName;
int systemPort;
struct systemList *top;
int maxListNum;
fd_set readSet, allSet;
int maxFD;
int IS_REGISTERED = 0;
int maxConnection = 0;

void startClient(int inputPort){
	systemIP = (char*)malloc(sizeof(char)*INET6_ADDRSTRLEN);
		checkAlloc(systemIP);
	systemName = (char*)malloc(sizeof(char)*INET6_ADDRSTRLEN);
		checkAlloc(systemName);
	memset(systemIP,'\0',INET6_ADDRSTRLEN);
	memset(systemName,'\0',INET6_ADDRSTRLEN);

	systemPort = inputPort;

	
	strcpy(systemIP,findMyIP());
	logEntry("IP Address HOST: ",systemIP,I);
	
	strcpy(systemName,findMyName());
	logEntry("System Name HOST: ",systemName,I);

	top = addList(top,systemName,systemIP,inputPort,2,RESTRICTED);
	log_ret("Details added in List: ",D);


	int listenFD,accFD;

	listenFD = Socket(AF_INET,SOCK_STREAM,0);
		char cSock[PG];
		memset(cSock,'\0',PG);
		sprintf(cSock,"%d",listenFD);

	logEntry("Socket Created: ",cSock,N);

	struct sockaddr_in svrAddr,cliAddr;
	bzero(&svrAddr,sizeof(svrAddr));
	svrAddr.sin_family = AF_UNSPEC;
	svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svrAddr.sin_port=htons(inputPort);

	Bind(listenFD,(SA*)&svrAddr,sizeof(svrAddr));
	log_ret("Binding Socket",N);


	Listen(listenFD,LISTENQ);

	maxFD = listenFD;

	
	FD_ZERO(&allSet);
	FD_SET(STDIN,&allSet);					//Registering the standard input file descriptor
	FD_SET(listenFD,&allSet);
	char command[MAX_STR_SIZE],recvMsg[MAX_STR_SIZE];
	int nready,i,j=0,timeout;
	socklen_t addrLen;

	int numBytes; 


	C_PROMPT;

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
					fgets (command, MAX_STR_SIZE, stdin);
					clientOps(command,commandParser(command),listenFD);
						memset(command,'\0',MAX_STR_SIZE);
					C_PROMPT;

				}else if(nready==listenFD){
					log_ret("New Connection ",N);	//Got a conncetion
					addrLen = sizeof(cliAddr);
					accFD = Accept(listenFD,(SA*)&cliAddr,&addrLen);
					/*extractInfoAndAdd(cliAddr,top,maxListNum,accFD);
					maxListNum++;*/
					char connAddress[INET6_ADDRSTRLEN];
						memset(connAddress,'\0',INET6_ADDRSTRLEN);

					struct sockaddr_in *sAdd = (struct sockaddr_in *)&cliAddr;

					inet_ntop(AF_INET, &sAdd->sin_addr, connAddress, sizeof connAddress);
					//printf("\nConnection request from: %s\n",connAddress);
					logEntry("CONN from: ",connAddress,N);

					maxFD = max(maxFD,accFD);
					FD_SET(accFD,&allSet);

				}else{
					
					numBytes = Recv(nready,recvMsg,MAX_STR_SIZE,0);
					if(numBytes==0){
						/*log_ret("Connection implicitly close",N);
						FD_CLR(nready,&allSet);
						close(nready);
						deleteEntry(nready,top);
							memset(recvMsg,'\0',MAX_STR_SIZE);*/
					}else{
						logEntry("Msg from connection ",recvMsg,N);
						/*printf("\nrecvMsg: %s\n",recvMsg );
						C_PROMPT;*/
						if(connMessageDecode(recvMsg,nready,top)==REGISTER){
							IS_REGISTERED = 1;
						}
						/*log_ret("Received a message from a connection",N);
						requestPad(recvMsg,nready,top);*/

						memset(recvMsg,'\0',MAX_STR_SIZE);
					}
				}
			}
		}
	}
}

void clientOps(char *command, int decision, int listenFD){

	switch(decision){
		case CREATOR : 	creator();
						break;
		case MYIP:		printf("IP Address: %s\n",systemIP);
						log_ret("CMD: IP Display",I);
						break;
		case MYPORT:	printf("Port: %d\n",systemPort);
						log_ret("CMD: Port Display",I);
						break;
		case HELP: 		cli_help();
						break;
		case LIST: 		displayList(top);
						log_ret("CMD: List",I);
						break;
		case EXIT:		close(listenFD);
						log_ret("CMD: Exit",I);
						Exit(EXIT_APP);
						break;
		case REGISTER:	log_ret("CMD: Register",I);
						registerConnection(command);
						break;
		case CONNECT:	log_ret("CMD: Connect",I);
						connectToHost(command);
		case BLANK:		break;
		default:		WRONG_COMMAND;
	}
}






void registerConnection(char *command){
	
	if(IS_REGISTERED==1){
		log_ret("Attempted Re-registration",I);
		printf("Already Registered: \n");
		return;
	}


	int i,val;
	char ent_IP[INET6_ADDRSTRLEN];
	char ent_Port[NG];
	
		memset(ent_IP,'\0',INET6_ADDRSTRLEN);
		memset(ent_Port,'\0',NG);
	strcpy(ent_IP,sepExtractor(command,' ',2));
	strcpy(ent_Port,sepExtractor(command,' ',LAST));

	//printf("%s and %s\n",ent_IP,ent_Port);

	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	Getaddrinfo(ent_IP, ent_Port, &hints, &res);
	int sockFD = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		char cSock[PG];
		memset(cSock,'\0',PG);
	
	maxFD = max(maxFD,sockFD);
	FD_SET(sockFD,&allSet);
	
	sprintf(cSock,"%d",sockFD);

	logEntry("Register- Sock created: ",cSock,N);
	
    Connect(sockFD, res->ai_addr, res->ai_addrlen);
	//printf("client: connecting to %s with port %s\n", ent_IP,ent_Port );
	char strRegist[MAX_STR_SIZE];
		memset(strRegist,'\0',MAX_STR_SIZE);

	sprintf(strRegist,"REGISTER-%s=%d=%s=",systemIP,systemPort,systemName);
	//printf("%s\n",strRegist);
	Send(sockFD,strRegist,strlen(strRegist),0);
}


void connectToHost(char *command){
	if(maxConnection>3){
		printf("Cannot connect with more than THREE peers\n");
		return;
	}

	int i;
	char ent_Host[MAX_STR_SIZE];
	char ent_Port[NG];
		memset(ent_Host,'\0',MAX_STR_SIZE);
		memset(ent_Port,'\0',NG);

	strcpy(ent_Host,sepExtractor(command,' ',2));
	strcpy(ent_Port,sepExtractor(command,' ',LAST));

	if(strcmp(ent_Host,systemIP)==0 || strcmp(ent_Host,systemName)==0){
		printf("Cannot connect to self\n");
		return;
	}

	if(validateConnectionParameters(ent_Host,ent_Port,top)==1){
		//All good to proceed
		struct addrinfo hints, *res;

		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		Getaddrinfo(ent_Host, ent_Port, &hints, &res);
		int sockFD = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			char cSock[PG];
			memset(cSock,'\0',PG);
		
		maxFD = max(maxFD,sockFD);
		FD_SET(sockFD,&allSet);
		
		sprintf(cSock,"%d",sockFD);

		logEntry("Connect- Sock created: ",cSock,N);
	
    	Connect(sockFD, res->ai_addr, res->ai_addrlen);

    	char strRegist[MAX_STR_SIZE];
		memset(strRegist,'\0',MAX_STR_SIZE);

		sprintf(strRegist,"CONNECT-%s=%d=%s=",systemIP,systemPort,systemName);
		//printf("%s\n",strRegist);
		Send(sockFD,strRegist,strlen(strRegist),0);

		maxConnection++;
	}else{
		
		return;
	}
}