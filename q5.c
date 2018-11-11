#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main(int argsc,char* argv[]){

	pid_t pid;
	int fd[2];
	
	/*char key[1000],file[1000];
		
	//printf("Enter keyword:");
	scanf("%s",key);
	//printf("Enter filename:");
	scanf("%s",file);*/
	
	pipe(fd);
	pid=fork();
	
	if(pid==0){
	
		dup2(fd[1],1);
		close(fd[0]);
		
		
		
		execlp("grep","grep","-n",argv[1],argv[2],NULL);
		exit(0);
		
	}
	else{
	
		dup2(fd[0],0);
		close(fd[1]);
		
		execlp("cut","cut","-d",":","-f","1",NULL);
	}
}




