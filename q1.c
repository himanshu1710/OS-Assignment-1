#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>

#define clear() printf("\033[H\033[J")

void sigHandler(int sig){
    signal(sig, sigHandler);
    printf(" \nEnter \"quit\" to exit prompt \n");
    startNewExec();
    fflush(stdout);
    return;
}

void startNewExec(){

	printf("myshell>> ");
}

void initShell(){

	clear();
	printf("/////////////////////////////\n");
	printf("//                         //\n");
	printf("//        My Shell         //\n");
	printf("//                         //\n");
	printf("/////////////////////////////\n\n\n");
	
}

void execcd(char *input,char dels[]){

	char* args = strsep(&input,dels);
	char cwd[1000];
	
	
	if(args==NULL){

		getcwd(cwd,sizeof(cwd));
		printf("%s\n", cwd);
			
	}
	else{
	
		int flag = chdir(args);
		
		if (flag == 0) {
			getcwd(cwd,sizeof(cwd));
		    	printf("%s\n",cwd);
		} 
		else
		    printf("Directory does not exist.\n");		
	}
			
		        		
			
}

void exececho(char* input,char dels[]){

	pid_t pid;
	pid = fork();
	
	if(pid==0){
	
		char *args = strsep(&input,dels);
		execlp("/bin/echo","echo",args,NULL);
		exit(0);
	}
	else{
		
		wait(NULL);	
	}
}

void execls(char* input,char dels[]){

	pid_t pid;
	pid = fork();
	
	if(pid==0){
	
		char *args = strsep(&input,dels);
		execlp("/bin/ls","ls",args,NULL);
		exit(0);
	}
	else{
		
		wait(NULL);	
	}
}

void execwc(char* input,char dels[]){

	pid_t pid;
	pid = fork();
	
	if(pid==0){
	
		char *option = strsep(&input,dels);
		if(option == NULL || *option!='-'){
		
			printf("Enter option.\n");
			return;
		}
		char *file = strsep(&input,dels);
		if(file == NULL){
		
			printf("Enter filename.\n");
			return;
		}
		execlp("/usr/bin/wc","wc",option,file,NULL);
		exit(0);
	}
	else{
		
		wait(NULL);	
	}
}

int main(){

	signal(SIGINT,sigHandler);
	initShell();
	char command[1000];
	char dels[]=" /t";
	while(1){
	
		startNewExec();
		gets(command);
		
		char* input;
		input = command;
		if(strcmp(command,"quit")==0){
			
			break;
		}
		char *commandType = strsep(&input,dels);
		if(strcmp(commandType,"cd")==0){
			
			execcd(input,dels);
		}
		else if(strcmp(commandType,"echo")==0){
		
			exececho(input,dels);
		}
		else if(strcmp(commandType,"ls")==0){
		
			execls(input,dels);
		}
		else if(strcmp(commandType,"wc")==0){
		
			execwc(input,dels);
		}
		else if(strcmp(commandType,"clear")==0){
		
			clear();
		}
		else{
		
			printf("Command does not exist.\n");
		}
	}
	
	
}
