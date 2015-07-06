/*
 * FileName: ft_helper.c
 * Author: Srinivasan Rajappa
 * Created: 06 June, 2015
 * Description: This C file contains helper functions that will be used in mnc_final.c
 * The document shall be updated if more functions are requried.  
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
		//err_ret("Incorrect Number of Arguments: \n\t Correct Format: ./app MODE XXXX\n");
		log_ret("validateInput: Incorrect Number of Arguments",E);
		return RESTRICTED;
	}
}

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

char* findMyIP(){
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
	return(myIPAddress); 
}


void checkAlloc(void *ptr){
	if(ptr==NULL){
		log_ret("char_mallMem: Out of Memory to allocate",E);
		EXIT_MESSAGE;
		Exit(ERR_TRANS);
	}
}


// References: http://stackoverflow.com/questions/504810/how-do-i-find-the-current-machines-full-hostname-in-c-hostname-and-domain-info

char *findMyName(){
	struct addrinfo hints, *info, *p;
	int i=0;
	char hostname[MAX_STR_SIZE];
		memset(hostname,'\0',MAX_STR_SIZE);
	
	gethostname(hostname, MAX_STR_SIZE);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; /*either IPV4 or IPV6*/
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_CANONNAME;

	Getaddrinfo(hostname,"http",&hints,&info);

	for(p = info; p != NULL; p = p->ai_next) {
	    return(p->ai_canonname);
	}
}

void SystemList(struct systemList *ptr){
	ptr->name = NULL;
	ptr->IPAddress = NULL;
	ptr->portNum = 0; 
	ptr->serialNum =0;
	ptr->next = NULL;
}

void addToList(struct systemList *ptr, char *name, char *IPAddress,int portNum, int serialNum,int sockFD){
	struct systemList *temp;
	temp = ptr;
	while(temp->next!=NULL){
		temp = temp->next;
	}
	temp->name = (char*)malloc(sizeof(char)*MAX_STR_SIZE);
		checkAlloc(temp->name);
		memset(temp->name,'\0',MAX_STR_SIZE);
	strcpy(temp->name,name);

	temp->IPAddress = (char*)malloc(sizeof(char)*MAX_STR_SIZE);
		checkAlloc(temp->IPAddress);
		memset(temp->IPAddress,'\0',MAX_STR_SIZE);
	strcpy(temp->IPAddress,IPAddress);

	temp->portNum = portNum;
	temp->serialNum = serialNum;
	temp->sockFD = sockFD;
	temp->next = (struct systemList*) malloc(sizeof(struct systemList));
	SystemList(temp->next);
}

void displayList(struct systemList *ptr){
	log_ret("CMD: List",I);
	struct systemList *temp;
	temp = ptr;
	while(temp->next!=NULL){
		printf("%-5d%-35s%-20s%-8d\n",temp->serialNum,temp->name,temp->IPAddress,temp->portNum);
		if(temp->next->next == NULL)
			break;
		else
			temp = temp->next; 
	}
}

int commandParser(char *command){
	command = strtok(command,"\n");
	if(strcmp(Toupper(command),"CREATOR")==0){
		return CREATOR;
	}
	if(strcmp(Toupper(command),"HELP")==0){
		return HELP;
	}
	if(strcmp(Toupper(command),"MYIP")==0){
		return MYIP;
	}
	if(strcmp(Toupper(command),"MYPORT")==0){
		return MYPORT;
	}
	if(strcmp(Toupper(command),"REGISTER")==0){
		return REGISTER;
	}
	if(strcmp(Toupper(command),"LIST")==0){
		return LIST;
	}
	if(strcmp(Toupper(command),"TERMINATE")==0){
		return TERMINATE;
	}
	if(strcmp(Toupper(command),"EXIT")==0){
		return EXIT;
	}
	if(strcmp(Toupper(command),"UPLOAD")==0){
		return UPLOAD;
	}
	if(strcmp(Toupper(command),"DOWNLOAD")==0){
		return DOWNLOAD;
	}
	if(strcmp(Toupper(command),"STATISTICS")==0){
		return STATISTICS;
	}

	return RESTRICTED;
}

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

void extractInfoAndAdd(struct sockaddr_in cliAddr, struct systemList *top, int serial, int sockFD){
	char cliIPAddress[INET6_ADDRSTRLEN];
	char cliName[MAX_STR_SIZE];
	struct addrinfo *p,hints,*info;
	inet_ntop(cliAddr.sin_family,(SA*)&cliAddr,cliIPAddress,INET6_ADDRSTRLEN);

	Getaddrinfo(cliIPAddress,"http",&hints,&info);
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_CANONNAME;

	for(p = info; p != NULL; p = p->ai_next) {
	    strcpy(cliName,p->ai_canonname);
	}
	addToList(top,cliName,cliIPAddress,cliAddr.sin_port,serial++,sockFD);
	log_ret("New connection details entered",I);
}

void sendUpdate(int socketFD, struct systemList *top){
	char updateString[MAX_STR_SIZE];
		memset(updateString,'\0',MAX_STR_SIZE);
	struct systemList *temp;
	strcpy(updateString,"UPDATE-");
	temp = top;
	while (temp->next !=NULL){
		strcat(updateString,"<");

			strcat(updateString,"[");
			strcat(updateString,intToString(temp->serialNum));
			strcat(updateString,"]");

			strcat(updateString,"[");
			strcat(updateString,temp->name);
			strcat(updateString,"]");

			strcat(updateString,"[");
			strcat(updateString,temp->IPAddress);
			strcat(updateString,"]");

			strcat(updateString,"[");
			strcat(updateString,intToString(temp->portNum));
			strcat(updateString,"]");

		strcat(updateString,">");
		if(temp->next->next==NULL)
			break;
	}

	Send(socketFD,updateString,strlen(updateString),0);
}

char *intToString(int number){
	char *str_number;
	str_number = (char*)malloc(sizeof(char)*MG);
		checkAlloc(str_number);
		memset(str_number,'\0',MG);
	int deg=10,i=0,val=0;
	while(number!=0){
		val = number%deg;
		str_number[i]= intToChar(val);
		i++;
		number/=deg;
	}
	str_number=strrev(str_number);
	return str_number;
}

char intToChar(int val){
	switch(val){
		case 0: return '0';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
		default: printf("intToChar: something wrong\n");
	}
}

char *strrev(char *s){
    int i,n;
    char *reverse;

    for(i=0;s[i]!='\0';i++);

    reverse=malloc(i*sizeof(char));
    n=i-1;
    for(i=n;i>=0;i--)
        reverse[n-i]=s[i];
    
    reverse[n+1]='\0';
    return reverse;
}

void periodicUpdate(struct systemList *top){
	struct systemList *temp;
	temp = top;
	while(temp->next!=NULL){
		if(temp->next!=RESTRICTED){
			sendUpdate(temp->sockFD,top);
		}if(temp->next->next==NULL){
			break;
		}
	}
}