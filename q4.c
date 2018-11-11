#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>


void exec(char* k){

	pid_t pid;
	int fd[2];
	pipe(fd);
	pid = fork();
	
	if(pid==0){
	
		int fd1[2];
		pipe(fd1);
		pid = fork();
		
		if(pid==0){
		
			dup2(fd1[1],1);
			close(fd1[0]);
			
			
			execlp("ps","ps","-eo","pcpu,pid",NULL);
			exit(0);
			
		}
		else{
		
			wait(NULL);
			
			dup2(fd1[0],0);
			dup2(fd[1],1);
			close(fd1[1]);
			close(fd[0]);
			
			execlp("sort","sort","-r",NULL);
			exit(0);
			
		}
	}
	else{
	
		wait(NULL);
		
		dup2(fd[0],0);
		close(fd[1]);
		
		execlp("head","head","-n",k,NULL);
		
	}
}

void reverse(char* num,int len){

	int i;
	for(i=0;i<len/2;i++){
	
		char ch;
		ch=num[i];
		num[i]=num[len-i-1];
		num[len-i-1]=ch;
	}
}

int main(){

	int n,k,r,rem,len=0;
	printf("Enter n k r:");
	scanf("%d %d %d",&n,&k,&r);
	k++;
	
	char num[1000];
	while(k!=0){
	
		rem=k%10;
		num[len++]=(char)(rem+48);
		k=k/10;
	}
	num[len]='\0';
	reverse(num,len);
	
	int counter=0;
	int fd[2];
	pipe(fd);
	
	pid_t pid;
	pid=fork();
	
	if(pid==0){
		
		printf("CHILD\n");
		while(1){
			
			close(fd[1]);
			int msg=0;
			read(fd[0],&msg,sizeof(msg));
			if(msg!=0){
			
				printf("Kill request has arrived.\n");
				if(msg==-1){
				
					printf("No process killed.\n");
				}
				else{
					char num[1000];
					int len=0,rem;
					
					while(msg!=0){
					
						rem=msg%10;
						msg=msg/10;
						num[len++] = (char)(rem+48);
					}
					num[len]='\0';
					reverse(num,len);
					printf("%s\n",num);
					
					if(fork()==0){
					
						execlp("kill","kill",num,NULL);
					}
					else{
					
						wait(NULL);
						printf("Killed process.\n");
					}
					
				}
			}
			else{
			
				sleep(n);
			}
			
			
		}
	}
	else{

		while(1){
		
			printf("PARENT\n");
			if(counter==r){
			
				counter=0;
				int msg;
				printf("Enter pid of process to be killed:\n");
				scanf("%d",&msg);
				close(fd[0]);
				write(fd[1],&msg,sizeof(msg));
				sleep(n);
			}
			else{
				
				counter++;
				if(fork()==0){
				
					exec(num);	
				}
				else{
				
					wait(NULL);
				}
				sleep(n);
				
			}
		}
		printf("Parent terminated\n");
		
	}
		
	
}
