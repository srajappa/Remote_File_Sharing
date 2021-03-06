/*
 * FileName: ft_helper.c
 * Author: Srinivasan Rajappa
 * Created: 06 June, 2015
 * Description: This C file contains helper functions that will be used in mnc_final.c
 * The document shall be updated if more functions are requried.  
 * FIVE BROAD SECTIONS:
 * 1) STRING RELATED
 * 2) LINKED LIST RELATED
 * 3) GENERAL OPERATIONS
 * 4) MISC UTILITIES
 * 5) SOCKET OPS
 * 6) NETWORK COMMUNICATIONS
 * ----REFERENCES-----
 * The motivation for creating this file was obtained after going through the material in the book
 * Unix Networking Programming (written by W. Richard Stevens). 
 * Edited (Srinivasan): 19 June, 2015 (Git branch: revelation1)
 *		  (Srinivasan): 03 July, 2015 
 */
#include "help.h"
#include "mnc.h"
#include "error.h"
#include "netinet/in.h"

int I_PORT;

char svrIP[INET6_ADDRSTRLEN];
int svrPort,startMin, startSec,file_Size;
char logString[MAX_STR_SIZE];
char myName[MG];
//STRING OPS --------------------------- BEGIN

char *Toupper(char*strVal){
	int i=0; 
	static char tmpStr[MAXSIZE];
	strcpy(tmpStr,strVal);
	while(tmpStr[i]){
		tmpStr[i] = toupper(tmpStr[i]);
		i++;
	}
	return(tmpStr);
}

char *sepExtractor(char*line,char symbol, int num){
	int i; 
	int fCom=0,lCom,flag = 0;

	if(num==LAST){
		char *resultl = malloc(sizeof(char)*MG);
			memset(resultl,'\0',MG);
		int p=0; 
		for(i=strlen(line)-1; i>=0;i--){
			if(line[i]==symbol){
				flag =1;
				fCom = i+1;
				break;
			}
		}
		if(flag==1){
			for(i=fCom; i < strlen(line); i++){
				resultl[p++] = line[i];
			}
			return resultl;
		}else{
			return "NULL";
		}
	}

	if(line[0]==symbol){
		if(num==1){
			return "NULL";
		}else{
			return strtok(line,",- ");
		}
	}else{
		for(i=0;i<strlen(line);i++){
			if(line[i]==symbol){
				lCom = i;
				num--;
				flag =1;
				if(num==0){
					break;
				}else{
					fCom = lCom+1;
				}
			}else{
				flag = 0; 
			}
		}
		if(flag==0)
			return line;
		else{
			char *result = (char*)malloc(sizeof(char)*MG);
			memset(result,'\0',MG);
			int k =0; 
			for(i=fCom; i<lCom; i++){
				result[k] = line[i];
				k++;
			}
			//printf("RESULT : %s\n",result );
			return result;
		}
	}
	return NULL;
}




int commandParser(char *command){
	char tempCmd[MG];
		memset(tempCmd,'\0',MG);
	if(strlen(command)==1){
		return BLANK;
	}
	command = strtok(command,"\n");
	strcpy(tempCmd,sepExtractor(command,' ',1));

	if(strcmp(Toupper(tempCmd),"CREATOR")==0){
		return CREATOR;
	}
	if(strcmp(Toupper(tempCmd),"HELP")==0){
		return HELP;
	}
	if(strcmp(Toupper(tempCmd),"MYIP")==0){
		return MYIP;
	}
	if(strcmp(Toupper(tempCmd),"MYPORT")==0){
		return MYPORT;
	}
	if(strcmp(Toupper(tempCmd),"REGISTER")==0){
		return REGISTER;
	}
	if(strcmp(Toupper(tempCmd),"CONNECT")==0){
		return CONNECT;
	}
	if(strcmp(Toupper(tempCmd),"LIST")==0){
		return LIST;
	}
	if(strcmp(Toupper(tempCmd),"TERMINATE")==0){
		return TERMINATE;
	}
	if(strcmp(Toupper(tempCmd),"EXIT")==0){
		return EXIT;
	}
	if(strcmp(Toupper(tempCmd),"UPLOAD")==0){
		return UPLOAD;
	}
	if(strcmp(Toupper(tempCmd),"DOWNLOAD")==0){
		return DOWNLOAD;
	}
	if(strcmp(Toupper(tempCmd),"STATISTICS")==0){
		return STATISTICS;
	}

	return RESTRICTED;
}



//STRING OPS ___________________________ END

//LINKED LIST -------------------------- BEGIN

void displayList(struct systemList *top){
	if(top==NULL)
		fprintf(stderr, "%s\n","displayList: Data Absent" );
	struct systemList *temp;
	temp = top;
	while(temp!=NULL){
		printf("%-5d%-35s%-20s%-8d\n",temp->serialNum,temp->name,temp->IPAddress,temp->portNum);
		temp = temp->next;
	}
}



struct systemList *addList(struct systemList *top, char *name, char *IPAddress, int portNum, int serialNum, int connFD){
	if(top==NULL){

		top = (struct systemList *)malloc(sizeof(struct systemList));
			checkAlloc(top);
		top->IPAddress = (char*)malloc(sizeof(char)*INET6_ADDRSTRLEN);
			checkAlloc(top->IPAddress);
			memset(top->IPAddress,'\0',INET6_ADDRSTRLEN);
		top->name = (char *)malloc(sizeof(char)*MAX_STR_SIZE);
			checkAlloc(top->name);
			memset(top->name,'\0',MAX_STR_SIZE);
		strcpy(top->name,name);
		strcpy(top->IPAddress, IPAddress);
		top->portNum = portNum;
		top->serialNum = serialNum;
		top->connFD = connFD;

		top->next = NULL;	
		log_ret("Entry Added in Linked List",I);
		return top;

	}else{
		struct systemList *temp;
		temp = top;
		while(temp->next != NULL){
			temp = temp->next;
		}
		temp->next = (struct systemList *)malloc(sizeof(struct systemList));
			checkAlloc(temp->next);
		temp = temp->next;

		temp->IPAddress = (char*)malloc(sizeof(char)*INET6_ADDRSTRLEN);
			checkAlloc(temp->IPAddress);
			memset(temp->IPAddress,'\0',INET6_ADDRSTRLEN);
		temp->name = (char *)malloc(sizeof(char)*MAX_STR_SIZE);
			checkAlloc(temp->name);
			memset(temp->name,'\0',MAX_STR_SIZE);
		strcpy(temp->name,name);
		strcpy(temp->IPAddress, IPAddress);
		temp->portNum = portNum;
		temp->serialNum = serialNum;
		temp->connFD = connFD;

		temp->next = NULL;
		log_ret("Entry Added in Linked List",I);
		return top;
	}
	
}


struct systemList * rearrangeList(struct systemList *top){
	struct systemList *tj,*tj1;
	int swapInt;
	char *swapString = (char*)malloc(sizeof(char)*MAX_STR_SIZE);
		memset(swapString,'\0',MAX_STR_SIZE);

	tj = top;
	tj1= top->next;

	
		swapInt = tj1->serialNum;
		tj1->serialNum = tj->serialNum;
		tj->serialNum = swapInt;

		swapInt = tj1->portNum;
		tj1->portNum = tj->portNum;
		tj->portNum = swapInt;

		swapInt = tj1->connFD;
		tj1->connFD = tj->connFD;
		tj->connFD = swapInt;

		strcpy(swapString,tj1->name);
			memset(tj1->name,'\0',MAX_STR_SIZE);
		strcpy(tj1->name,tj->name);
			memset(tj->name,'\0',MAX_STR_SIZE);
		strcpy(tj->name,swapString);

		memset(swapString,'\0',MAX_STR_SIZE);

		strcpy(swapString,tj1->IPAddress);
			memset(tj1->IPAddress,'\0',INET6_ADDRSTRLEN);
		strcpy(tj1->IPAddress,tj->IPAddress);
			memset(tj->IPAddress,'\0',INET6_ADDRSTRLEN);
		strcpy(tj->IPAddress,swapString);

		memset(swapString,'\0',MAX_STR_SIZE);

	
	//free(swapString);
	return top;
}

int validateConnectionParameters(char *ent_Host,char *ent_Port,struct systemList *top){
	//Validate the input
	//1. Identify whether Host name / IP Address.
	//2. Validate the host detail has conncetion ID as RESTRICTED
	//3. Validate Port details if all pass then fine
	int i, mode;
	int flag = 0;
	struct systemList *temp;
	for(temp = top ; temp!= NULL; temp = temp->next){
		if(strcmp(ent_Host,temp->IPAddress)==0 || strcmp(ent_Host,temp->name)==0){
			//printf("CONNFD %d\n", );
			if(temp->connFD==RESTRICTED){		//CONN ID
				if(temp->portNum==atoi(ent_Port)){	//PORT MATCHES
					flag = 1;
				}else{
					printf("Port number do not match\n");
					break;
				}
			}else{
				printf("Connection Already Established- Cannot reestablish connection\n");
				break;
			}
		}
	}
	if(flag==0){
		printf("Host information not in SERVER-IP List\n");
	}
	return flag;
}

struct systemList *deleteNode(struct systemList *top, char *guestName){
	struct systemList *temp, *t1;
	t1 = top;
	for(temp = top->next; temp!=NULL; temp=temp->next){
		if(strcmp(guestName,temp->name)==0){
			t1->next = temp->next;
			logEntry("Link Disassociated: ",temp->name,I);
			break;
		}
		t1 = temp;
	}
	if(strcmp(guestName,top->name)==0){
		top = top->next;
	}
	return top;
}

//LINKED LIST __________________________ END


//GENERAL OPERATIONS -------------------------- BEGIN

void creator(){
	log_ret("CMD: Creator",I);
	printf("%-30s%-20s%-30s \n","Srinivasan Rajappa","srajappa","srajappa@buffalo.edu");
	printf("I have read and understood the course academic integrity policy located at http://www.cse.buffalo.edu/faculty/dimitrio/courses/cse4589_f14/index.html#integrity\n");
}

void cli_help(){
	log_ret("CMD: CLIENT help",I);
	printf("%-10s%-15s%-50s\n","","MYIP:","Displays the IP Address of the current System");
	printf("%-10s%-15s%-50s\n","","MYPORT:","Displays the Port Number of the current System");
	printf("%-10s%-15s%-50s\n","","REGISTER:","Requires two arguments and performs a registrations to the server");
			printf("%-10s%-15s%-50s\n","","","<Param_1> Server IP Address ||||||| <Param_2> Port Number");
	printf("%-10s%-15s%-50s\n","","CONNECT:","Requires two arguments and creates a TCP connection between the system and the server");
			printf("%-10s%-15s%-50s\n","","","<Param_1> Either Server IP Address OR canonical Name ||||||| <Param_2> Port Number");
	printf("%-10s%-15s%-50s\n","","LIST:","Displays a list of Systems with whom an active connection is maintained");
	printf("%-10s%-15s%-50s\n","","TERMINATE:","Requires connection ID and removes a connection from the list");
	printf("%-10s%-15s%-50s\n","","EXIT:","Exits from all active participations");
	printf("%-10s%-15s%-50s\n","","UPLOAD:","Requires two arguments and it upload file to the indended recepient");
			printf("%-10s%-15s%-50s\n","","","<Param_1> Connection ID ||||||| <Param_2> File Name");
	printf("%-10s%-15s%-50s\n","","DOWNLOAD:","May have multiple arguments i.e. can be used to download files from many Systems");
	printf("%-10s%-15s%-50s\n","","STATISTICS:","Shows STATISTICS on what is the amount of data sent and received");
}

void svr_help(){
	log_ret("CMD: SVR help",I);
	printf("%-10s%-15s%-50s\n","","MYIP:","Displays the IP Address of the current System");
	printf("%-10s%-15s%-50s\n","","MYPORT:","Displays the Port Number of the current System");
	printf("%-10s%-15s%-50s\n","","CONNECT:","Requires two arguments and creates a TCP connection between the system and the server");
			printf("%-10s%-15s%-50s\n","","","<Param_1> Either Server IP Address OR canonical Name ||||||| <Param_2> Port Number");
	printf("%-10s%-15s%-50s\n","","LIST:","Displays a list of Systems with whom an active connection is maintained");
	printf("%-10s%-15s%-50s\n","","TERMINATE:","Requires connection ID and removes a connection from the list");
	printf("%-10s%-15s%-50s\n","","EXIT:","Exits from all active participations");
	printf("%-10s%-15s%-50s\n","","UPLOAD:","Requires two arguments and it upload file to the indended recepient");
			printf("%-10s%-15s%-50s\n","","","<Param_1> Connection ID ||||||| <Param_2> File Name");
	printf("%-10s%-15s%-50s\n","","STATISTICS:","Shows STATISTICS on what is the amount of data sent and received");
}


//GENERAL OPERATIONS ___________________________ END


//MISC UTILITIES ___________________________ BEGIN

int fileSize(FILE *fptr){
	char ch;
	int ctr=0;
	while(ch!=EOF){
		ch = getc(fptr);
		ctr++;
	}
	return ctr;
}


int validateInput(int argc, char *argv[]){
	if(argc==3){
		if((strcmp(Toupper(argv[1]),"S")==0)){
					log_ret("Identified: Server Mode",I);

			I_PORT=validatePort(argv[2]);

					log_ret("Identified: Port value",I);

			return SERVER_MODE;
		}else if((strcmp(Toupper(argv[1]),"C")==0)){
					log_ret("Identified: Client Mode",I);

			I_PORT=validatePort(argv[2]);
						
					log_ret("Identified: Port value",I);

			return CLIENT_MODE;
		}else{
			log_ret("Wrong number Argument list: ",E);
			return RESTRICTED;
		}
	}else{
		log_ret("validateInput: Incorrect Number of Arguments",E);
		return RESTRICTED;
	}
}



int validatePort(char *port){
	int i, isItNumber=0; 
	for(i=0; i<strlen(port); i++){
		if(port[i]>=48 && port[i]<=57){
			isItNumber = 1; 
		}else{
			isItNumber = 0;
			break;
		}
	}
	if(isItNumber==1){
		return (atoi(port));
	}else{
		log_ret("Port Value is not an integer number",E);
		printf("Program Terminated: \n\tIncorrect Input: < %s > is not a number \n",port);
		Exit(ERR_IO);
	}
}

void launch(int mode){
	if(mode==SERVER_MODE){
		log_ret("Starting the Server ",D);
		startServer(I_PORT);
		log_ret("Shutting Down the Server ",D);
	}else{
		log_ret("Starting the Client system",D);
		startClient(I_PORT);
		log_ret("Shutting Down Client system ",D);
	}
}


void checkAlloc(void *sample){
	if(sample==NULL){
		fprintf(stderr, "%s\n","checkAlloc: Not enough memx" );
		log_ret("checkAlloc: Not enough memx",E);
		Exit(ERR_ABNOR);
	}
}


int findID(struct systemList *top){
	
	if(top==NULL)
		return 1;
	int foundNum =1,i; 
	struct systemList *temp;
	for(i=1; i<= 10 ; i++){
		for(temp = top; temp != NULL; temp = temp->next){
			if(i==temp->serialNum){
				foundNum = 0;
				break;
			}
		}
		if(foundNum==0){
			foundNum = 1; 
			continue;
		}else{
			return i;
		}
	}
	
}

//MISC UTILITIES __________________________ END


//SOCKET OPS---------------------------BEGIN

char *findMyIP(){
	int sockfd;
	
	struct addrinfo hints,*res;
	struct sockaddr_in resv;
	int val;
	static char myIPAddress[INET6_ADDRSTRLEN];
	
	memset(&hints,0,sizeof hints);

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	
	Getaddrinfo(GOOGLE_IP,SERV_PORT_STR,&hints,&res);

	//Create a socket
	sockfd = Socket(res->ai_family,res->ai_socktype,0);

	//connect()
	Connect(sockfd,res->ai_addr,res->ai_addrlen);
	socklen_t len = sizeof(resv);
		
	Getsockname(sockfd,(struct sockaddr*)&resv,&len);

	strcpy(myIPAddress,inet_ntoa(resv.sin_addr));
	
	close(sockfd);

	if(strcmp(myIPAddress,"0.0.0.0")==0){
		LIMITED_CONNECTION;
		Exit(ERR_TRANS);
	}

	//

	return(myIPAddress); 
}


char *findMyName(){
	struct addrinfo hints, *info, *p;
	int i=0;
	char hostname[MAX_STR_SIZE];
		memset(hostname,'\0',MAX_STR_SIZE);
	
	gethostname(hostname, MAX_STR_SIZE);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; //either IPV4 or IPV6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_CANONNAME;

	Getaddrinfo(hostname,"http",&hints,&info);

	for(p = info; p != NULL; p = p->ai_next) {
		strcpy(myName,p->ai_canonname);
	    return(p->ai_canonname);
	}
}


//SOCKET OPS______________________________END


//NETWORK OPS---------------------------BEGIN

int connMessageDecode(char *recvMsg,int nready,struct systemList *top, int *maxconnection){

	int i,j=0;
	int ctr=0;
	char tempCmd[MAX_STR_SIZE];
		memset(tempCmd,'\0',MAX_STR_SIZE);
	char localB[MAX_STR_SIZE];
		memset(localB,'\0',MAX_STR_SIZE);
	char guestName[MG];
	char guestIP[MG];
		
	int guestPort,num;
	int retVal=0;
	FILE *fptr;


	struct systemList *temp = top;

	strcpy(tempCmd,sepExtractor(recvMsg,'-',1));

	if(strcmp(tempCmd,"REGISTER")==0){
		/*	1. Extract information about sender and 
			2. Add information to systemList
			3. Send Response indicating Confirmation
			4. Send Packet Update*/
		

		strcpy(guestName,sepExtractor(recvMsg,'=',3));
		strcpy(guestIP,sepExtractor(sepExtractor(recvMsg,'-',LAST),'=',1));

		guestPort = atoi(sepExtractor(recvMsg,'=',2));
		//--------------------------------------------
		num = findID(top);
		top = addList(top,guestName,guestIP,guestPort,num,nready);
		//--------------------------------------------
		char *sendInfo = (char*)malloc(sizeof(char)*MAX_STR_SIZE);
			memset(sendInfo,'\0',MAX_STR_SIZE);
		sprintf(sendInfo,"REG_ACK-%s=%d=%s=",top->IPAddress,top->portNum,top->name);

		Send(nready,sendInfo,strlen(sendInfo),0);

		sendUpdate(top);

	}else if(strcmp(tempCmd,"REG_ACK")==0){

		printf("Registration successfully completed.\n");
		C_PROMPT;
		log_ret("Registration Completed",I);
		/*	1. Extract information about sender and 
			2. Add information to systemList.
			3. Sort them.
		*/

		//--------------------------------------------
		

		strcpy(guestName,sepExtractor(recvMsg,'=',3));
		strcpy(guestIP,sepExtractor(sepExtractor(recvMsg,'-',LAST),'=',1));

		guestPort = atoi(sepExtractor(recvMsg,'=',2));
		//--------------------------------------------
		num = findID(top);
		top = addList(top,guestName,guestIP,guestPort,num,nready);
		top = rearrangeList(top);

		//IS_REGISTERED = 1;
		retVal = REGISTER;

	}else if(strcmp(tempCmd,"UPDATE")==0){
		
		logEntry("Msg from connection ",recvMsg,N);
			memset(tempCmd,'\0',MAX_STR_SIZE);
		strcpy(tempCmd,sepExtractor(recvMsg,'-',LAST));
		for(i=0; i<strlen(tempCmd); i++){
			if(tempCmd[i]=='!'){
				ctr++;
			}
		}
		printf("\n--Recvd updated SERVER-IP list--\n");

		for(i=1; i<=ctr; i++){
			strcpy(localB,sepExtractor(tempCmd,'!',i));
			if(strcmp(myName,sepExtractor(localB,'=',3))==0){
				memset(localB,'\0',MAX_STR_SIZE);
				
				continue;
			}else{
				strcpy(guestName,sepExtractor(localB,'=',3));
				strcpy(guestIP,sepExtractor(localB,'=',1));
				guestPort = atoi(sepExtractor(localB,'=',2));
				num = findID(top);
				//printf("GIP:%s GNAME:%s GPORT %d\n",guestIP, guestName,guestPort );

				top = addList(top,guestName,guestIP,guestPort,num,RESTRICTED);
				memset(localB,'\0',MAX_STR_SIZE);
				memset(guestName,'\0',MG);
				memset(guestIP,'\0',INET6_ADDRSTRLEN);				
			}
		}
		//printf("\n");
		displayList(top);
		
		C_PROMPT;
	

	}else if(strcmp(tempCmd,"CONNECT")==0){
		logEntry("Msg from connection ",recvMsg,N);
		strcpy(guestName,sepExtractor(recvMsg,'=',3));
		strcpy(guestIP,sepExtractor(sepExtractor(recvMsg,'-',LAST),'=',1));

		guestPort = atoi(sepExtractor(recvMsg,'=',2));

		for(temp = top; temp!=NULL; temp = temp->next){
			if(strcmp(temp->IPAddress,guestIP)==0){
				temp->connFD = nready;
				break;
			}
		}
		printf("\nConnection received from %s\n",guestName);
		logEntry("Connection by peer: ",guestName,N);
		//Send Acknowledgement
		char *sendInfo = (char*)malloc(sizeof(char)*MAX_STR_SIZE);
			memset(sendInfo,'\0',MAX_STR_SIZE);
		sprintf(sendInfo,"CONN_ACK-%s-%s-",top->next->IPAddress,top->next->name);

		Send(nready,sendInfo,strlen(sendInfo),0);

		memset(guestName,'\0',MG);
		memset(guestIP,'\0',INET6_ADDRSTRLEN);	
		C_PROMPT;
	

	}else if(strcmp(tempCmd,"CONN_ACK")==0){
		logEntry("Msg from connection ",recvMsg,N);
		strcpy(guestName,sepExtractor(recvMsg,'-',3));
		for(temp = top ; temp!=NULL; temp = temp->next){
			if(strcmp(guestName,temp->name)==0){
				temp->connFD = nready;
				break;
			}
		}
		printf("\nSuccessfully connected to : %s\n",sepExtractor(recvMsg,'-',3));
		logEntry("Connection successful PEER: ",sepExtractor(recvMsg,'-',3),N);
		memset(guestName,'\0',MG);
		C_PROMPT;
	

	}else if(strcmp(tempCmd,"TERMINATE")==0){
		logEntry("Msg from connection ",recvMsg,N);
		strcpy(guestName,sepExtractor(recvMsg,'=',3));

		for(temp=top; temp!=NULL; temp = temp->next){
			if(strcmp(temp->name,guestName)==0){
				logEntry("Connection terminated by",temp->name,N);
				printf("Connection terminated by %s\n",temp->name);
				break;
			}
		}
		memset(guestName,'\0',MG);
		C_PROMPT;
	

	}else if(strcmp(tempCmd,"EXIT")==0){
		logEntry("Msg from connection ",recvMsg,N);
		strcpy(guestName,sepExtractor(recvMsg,'=',3));

		for(temp=top; temp!=NULL; temp=temp->next){
			if(strcmp(temp->name,guestName)==0){

				top = deleteNode(top,guestName);
				deleteBroadcast(top,guestName);
				sendUpdate(top);
				printf("%s has exited the network. \n",temp->name);
				logEntry("Systems exits application:",temp->name,I);
				break;
			}
		}
		S_PROMPT;

	}else if(strcmp(tempCmd,"DELETE")==0){
		logEntry("Msg from connection ",recvMsg,N);
		strcpy(guestName,sepExtractor(recvMsg,'=',3));

		for(temp=top; temp!=NULL; temp=temp->next){
			if(strcmp(temp->name,guestName)==0){
				top = deleteNode(top,guestName);
				break;
			}
		}
		*maxconnection = *maxconnection-1;

	}else if(strcmp(tempCmd,"MASTEREXIT")==0){
		logEntry("Msg from connection ",recvMsg,N);
		printf("Connection to server lost, exiting network\n");
		log_ret("Master Exits",I);
		Exit(EXIT_MASTER);

	}else if(strcmp(tempCmd,"UPLOAD")==0){
		logEntry("Msg from connection ",recvMsg,N);
		log_ret("Upload requested",I);
		char fileName[MG];
			memset(fileName,'\0',MG);
		strcpy(fileName,sepExtractor(recvMsg,'-',3));
		//strcpy(fileName,"fileNew.txt");

		time_t now = time(NULL);
	   	struct tm *t = localtime(&now);
	   	
	   	startMin = t->tm_min;
	   	startSec = t->tm_sec;

	   	file_Size = atoi(sepExtractor(recvMsg,'-',4));
		fptr = fopen(fileName,"w");
		
		fflush(fptr);
		close(fptr);
	}else if(strcmp(tempCmd,"UPLOAD_FILE")==0){
		char fileName[MG];
			memset(fileName,'\0',MG);
		strcpy(fileName,sepExtractor(recvMsg,'-',3));
		//strcpy(fileName,"fileNew.txt");
		char paste[MAX_STR_SIZE];
			memset(paste,'\0',MAX_STR_SIZE);


		fptr = fopen(fileName,"a");
		for(i=0; i< strlen(recvMsg); i++){
			if(ctr<4){
				if(recvMsg[i]=='-'){
					ctr++;
				}
			}else{
				paste[j++]=recvMsg[i];
			}
		}

		fprintf(fptr,"%s",paste);
		fflush(fptr);
		close(fptr);
		logEntry("File updated: ",fileName,N);
		if(strcmp(sepExtractor(recvMsg,'-',4),"999")==0){
			printf("Downloaded file (%s) from %s\n",fileName,sepExtractor(recvMsg,'-',2));
			time_t now = time(NULL);
	   		struct tm *t = localtime(&now);

	   		int totalTime = (t->tm_min - startMin) * 60 + (t->tm_sec - startSec);

	   		totalTime = (totalTime==0) ? 1 : totalTime;
	   		//printf("TIME: starts %d and %d && now %d and %d\n",startMin, startSec, t->tm_min, t->tm_sec );
	   		int hmm = (file_Size*8)/totalTime; 
	   		printf("Rx: %s->%s, File size: %d Bytes, Time taken: %d seconds, Rx Rate: %d bits/sec \n",sepExtractor(sepExtractor(recvMsg,'-',2),'.',1),sepExtractor(myName,'.',1),file_Size,totalTime,hmm );

			logEntry("File write complete: ",fileName,N);
			C_PROMPT;
		}
	}


	return retVal;

}

void sendUpdate(struct systemList *top){
	struct systemList *temp;
	char *payLoad = (char*)malloc(sizeof(char)*MAX_STR_SIZE);
		memset(payLoad,'\0',MAX_STR_SIZE);
	char *tempStr = (char*)malloc(sizeof(char)*MAX_STR_SIZE);
		memset(tempStr,'\0',MAX_STR_SIZE);
	strcpy(payLoad,"UPDATE-");
	for(temp = top->next; temp!=NULL; temp = temp->next){
		sprintf(tempStr,"%s=%d=%s=!",temp->IPAddress,temp->portNum,temp->name);
		strcat(payLoad,tempStr); 	//Append new data;
			memset(tempStr,'\0',MAX_STR_SIZE);
	}
	logEntry("Update Payload: ",payLoad,I);
	for(temp = top->next; temp!=NULL; temp=temp->next){
		logEntry("Sending update to: ", temp->IPAddress, N);
		Send(temp->connFD,payLoad,strlen(payLoad),0);
	}
	//Srinivasan is here when register was completed
	/*free(payLoad);
	free(tempStr);*/
}


void deleteBroadcast(struct systemList *top, char *guestName){
	struct systemList *temp;
	char deleteString[MAX_STR_SIZE];
		memset(deleteString,'\0',MAX_STR_SIZE);
	sprintf(deleteString,"DELETE-&&&=444=%s=",guestName);
	for(temp=top->next; temp!=NULL; temp=temp->next){
		logEntry("Delete Broadcast to:",temp->name,N);
		Send(temp->connFD,deleteString,strlen(deleteString),0);
	}
}


void uploadFiles(char *command,struct systemList *top, fd_set *allSet){
	int connid = atoi(sepExtractor(command,' ',2));
	int startMin, startSec;
	int endMin, endSec;

	time_t now = time(NULL);
	struct tm *t = localtime(&now);

	if(connid==1){
		printf("Cannot Upload to SERVER\n");
		return;
	}
	if(connid==2){
		printf("Cannot Upload to self\n");
		return;
	}
	char fileName[MAX_STR_SIZE];
		memset(fileName,'\0',MAX_STR_SIZE);
	strcpy(fileName,sepExtractor(command,' ',LAST));		//Assuming the file is in the same directory

	FILE *fptr;
	fptr = fopen(fileName,"r");
	
	if(fptr==NULL){
		printf("Unable to read file, couldn't upload.\n");
		return;
	}

	int sizeOfFile = fileSize(fptr);
	int partitions = sizeOfFile / MAX_STR_SIZE;
	int recFlag = 0;
	int conn,i,j; 
	close(fptr);
	fptr = fopen(fileName,"r");

	startMin = t->tm_min;
	startSec = t->tm_sec;

	char tempStr[MG];
		memset(tempStr,'\0',MG);

	FD_CLR(STDIN,allSet);

	struct systemList *temp;
	char uploadString[MAX_STR_SIZE];
		memset(uploadString,'\0',MAX_STR_SIZE);
	sprintf(uploadString,"UPLOAD-%s-%s-%d-%d-%d-",myName,fileName,sizeOfFile,partitions+1,0);	//First partition
	//MY NAME, FILENAME, SIZEOFFILE, PARTITIONS, 


	for(temp=top; temp!=NULL; temp=temp->next){
		if(connid==temp->serialNum){
			recFlag = 1;
			conn = temp->connFD;
			logEntry("Uploading data to: ",temp->name,N);
			Send(conn, uploadString,sizeof(uploadString),0);
				memset(uploadString,'\0',MAX_STR_SIZE);
			break;
		}
	}
	if(recFlag==0){
		printf("Connection ID invalid, cannot upload file\n");
		return;
	}
	log_ret("Preparing for upload ",I);
	int ab =0;
	for(i=1; i<=partitions+1; i++){
		sprintf(uploadString,"UPLOAD_FILE-%s-%s-%d-",myName,fileName,ab=(i==partitions+1) ? 999 : i );
		for(j=(strlen(uploadString));j<MAX_STR_SIZE; j++){
			uploadString[j] = fgetc(fptr);
			if(uploadString[j]==EOF){
				uploadString[j]='\0';
				break;
			}
		}
		sprintf(tempStr,"%d--%s",i,temp->name,N);
		logEntry("Uploading payLoad #",tempStr,N);
		Send(conn,uploadString,sizeof(uploadString),0);
		memset(uploadString,'\0',MAX_STR_SIZE);
		memset(tempStr,'\0',MG);
	}
	close(fptr);
	printf("Uploaded file %s to %s\n",fileName,temp->name);
	int totalTime = (t->tm_min - startMin) * 60 + (t->tm_sec - startSec);

	totalTime = (totalTime==0) ? 1 : totalTime;
	//printf("TIME: starts %d and %d && now %d and %d\n",startMin, startSec, t->tm_min, t->tm_sec );
	int hmm = (sizeOfFile*8)/totalTime; 
	printf("Tx: %s->%s, File size: %d Bytes, Time taken: %d seconds, Rx Rate: %d bits/sec \n",sepExtractor(myName,'.',1),sepExtractor(temp->name,'.',1),sizeOfFile,totalTime,hmm );
	
	FD_SET(STDIN,allSet);
		//FD_SET(STDIN,&allSet);	
}
//NETWORK OPS______________________________END




