#ifndef __help_h
#define __help_h


#define SERVER_MODE 0
#define CLIENT_MODE 1
#define RESTRICTED -999
#define BLANK -99
#define PENDING -1000;

#define CREATOR		1
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
#define STATISTICS  12

#define ERRFILENAME "err_log.txt"
#define FILENAME "log.txt"
#define MG 50
#define NG 20
#define MAX_STR_SIZE 512
#define PG 6


#define FOUND 1
#define NOTFOUND -999
#define LAST -1

#define START 10
#define STOP -10

#define E 'E'
#define I 'I'
#define D 'D'
#define N 'N'

#define S_PROMPT printf("Svr@[PA-1]> "); fflush(stdout)
#define C_PROMPT printf("Cli@[PA-1]> "); fflush(stdout)



int validateInput(int, char*[]);
char *Toupper(char*);
char intToChar(int);
char *strrev(char *);
char *intToString(int );
char *sepExtractor(char*,char, int);


struct systemList
{
	char *name;
	char *IPAddress;
	int portNum;
	int serialNum;
	int connFD;
	struct systemList *next;
};

#endif