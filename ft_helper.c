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
int svrPort;
char logString[MAX_STR_SIZE];

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


//MISC UTILITIES __________________________ END


//SOCKET OPS---------------------------BEGIN

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
	    return(p->ai_canonname);
	}
}


//SOCKET OPS______________________________END







/*



void checkAlloc(void *ptr){
	if(ptr==NULL){
		log_ret("char_mallMem: Out of Memory to allocate",E);
		EXIT_MESSAGE;
		Exit(ERR_TRANS);
	}
}


// References: http://stackoverflow.com/questions/504810/how-do-i-find-the-current-machines-full-hostname-in-c-hostname-and-domain-info



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
	printf("\nNew Connection from : %s\n",cliName);
	S_PROMPT;
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

void deleteEntry(int fd, struct systemList *top){
	struct systemList *temp;
	temp = top;
	while(temp->next!=NULL){
		if(temp->next->sockFD == fd){
			temp = temp->next->next;
			break;
		}else{
			if(temp->next->next == NULL){
				break;
			}
			temp = temp->next;
		}
	}
	if(top->sockFD == fd){
		top = top->next;
	}
}

int strMatch(char *ch, char *comp){
	int i,j, k,l;
	i=j=k=l=0; 
	int check=0,ind=0;
	if(strlen(ch)<strlen(comp))
		return NOTFOUND;
	for(i=0; i<strlen(ch)-strlen(comp)+1; i++){
		k=i;
		for(j=0;j<strlen(comp);j++){
			if(ch[k]==comp[j]){
				k++;
				check++;
			}
			else{
				break;
			}
		}
		k=0;
		if(check==strlen(comp)){
			l = 1;
			break;
		}
		check=0;
	}

	return l==0 ?  NOTFOUND : FOUND;
}

struct systemList *findEntry(int sockFD, struct systemList*top){
	struct systemList *temp;
	temp = top;
	while(temp->next!=NULL){
		if(temp->sockFD == sockFD){
			return temp;
		}else{
			if(temp->next->next==NULL){
				break;
			}
			temp = temp->next;
		}
	}
}

void closeAllConnections(struct systemList *top){
	struct systemList *temp;
	if(top->next->next->next == NULL){
		log_ret("All connections closed",I);
	}else{
		temp = top->next->next;
		while(temp->next!=NULL){
			close(temp->sockFD);
			if(temp->next->next == NULL)
				break;
			temp = temp->next;
		}
		top->next->next = (struct systemList*)malloc(sizeof(struct systemList));
			checkAlloc(top->next->next);
		SystemList(top->next->next); 		//Retain the server and the system information	
	}

}

void requestPad(char *recvMsg, int sockFD, struct systemList *top){
	struct systemList *temp;
	char sendMsg[MAX_STR_SIZE];
		memset(sendMsg,'\0',MAX_STR_SIZE);
	
	if(strMatch(recvMsg,"TERMINATE")==FOUND){
		temp = findEntry(sockFD,top);
		printf("Termination requested by %s\n",temp->name);
		strcpy(sendMsg,"TER_ACK");
		Send(sockFD,sendMsg,strlen(sendMsg),0);
		deleteEntry(sockFD,top);
		periodicUpdate(top);
		log_ret("Termination Initiated ",N);
		close(sockFD);

	}else if(strMatch(recvMsg,"TER_ACK")==FOUND){
		printf("Termination completed by server\n");
		closeAllConnections(top);

	}else if(strMatch(recvMsg,"REGISTER")==FOUND){
		printf("Registration from a client\n");
		strcpy(sendMsg,"REG_ACK");
		Send(sockFD,sendMsg,strlen(sendMsg),0);
		informationAdd(recvMsg,top,sockFD);
		log_ret("System registered and added",I);

	}else if(strMatch(recvMsg,"REG_ACK")==FOUND){
		printf("Registration complete on server\n");
		log_ret("Registration complete on server",I);
	}
}

void informationAdd(char *recvMsg, struct systemList *top, int sockFD){
	int i,j=0;
	char t_IP[INET6_ADDRSTRLEN],t_Name[MG],t_Port[MG];
		memset(t_IP,'\0',INET6_ADDRSTRLEN);
		memset(t_Port,'\0',MG);
		memset(t_Name,'\0',MG);
	if(strMatch(recvMsg,"REGISTER")==FOUND){
		recvMsg = strtok(recvMsg,"REGISTER-");
		strcpy(t_IP,sepExtractor(recvMsg,'=',1));
		strcpy(t_Port,sepExtractor(recvMsg,'=',2));
		strcpy(t_Name,sepExtractor(recvMsg,'=',3));
		addToList(top,t_Name,t_IP,atoi(t_Port),maxSerial(top)+(rand()/10),sockFD);
	}
}

int maxSerial(struct systemList *top){
	struct systemList *temp;
	int max = 2;
	while(temp->next!=NULL){
		if(max < temp->serialNum){
			max = temp->serialNum
		}
		if(temp->next->next==NULL)
			break;
	}
	return max;
}



*/