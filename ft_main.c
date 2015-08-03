/*
* Program starts here, the input arguments are analysed and 
* corresponding mode is activated either server or client. 
* Later on exit, the program will terminate automatically.
* @author: Srinivasan Rajappa
*/
#include "mnc.h"
#include "error.h"
#include "help.h"

int main(int argc, char* argv[]){
	createLogFile();
	refresh(START);
	int mode;
	mode = validateInput(argc,argv);
	if(mode!=RESTRICTED){
		launch(mode);
	}else{
		printf("Program Terminated \n\tCorrect Format: ./app MODE XXXX\n");
		Exit(ERR_IO);
	}
	refresh(STOP);
	return 0;
}