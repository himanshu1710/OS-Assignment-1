#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<limits.h>

int isPrime(int n){

	int i,counter=0;
	for(i=1;i<=n/2;i++){

		if(n%i==0)
			counter++;
	}
	if(counter==1)
		return 1;

	else
		return 0;
}

int main(){

	pid_t pid;
	int fd[2];
	pipe(fd);
	pid=fork();

	if(pid==0){
		//printf("Child running\n");

		int vis[1000],i;
		for(i=0;i<1000;i++){

			vis[i]=0;
		}

		int input,sum=0;
		while(1){


			close(fd[1]);
			read(fd[0],&input,sizeof(input));

			printf("Child got %d\n",input);
			if(input==INT_MAX){

				printf("Child terminated\n");
				exit(0);
			}
			if(isPrime(input)){
				printf("%d is a prime\n",input);

				if(vis[input]==0){

					vis[input]=1;
					sum+=input;
				}

				printf("Sum till now: %d\n",sum);

			}
			sleep(input%3);

		}


	}
	else{

		int n;
		printf("Enter size of the array:");
		scanf("%d",&n);
		int a[n],i,vis[n];

		for(i=0;i<n;i++)
			vis[i]=0;

		printf("Enter elements of the array:");
		for(i=0;i<n;i++){

			scanf("%d",&a[i]);
		}

		//printf("Parent running\n");
		while(1){


			close(fd[0]);

			int flag=0;

			for(i=0;i<n;i++){

				if(vis[i]==0){

					flag=1;
					break;
				}
			}

			if(flag==0){

				int e=INT_MAX;
				write(fd[1],&e,sizeof(e));
				printf("Parent terminated\n");
				exit(0);
			}
			else{
				int x=0,y=0;

				do{
					x=rand()%n;
					y=rand()%n;

				}while(x==y);

				vis[x]=1;
				vis[y]=1;
				printf("Parent sent %d %d\n",a[x],a[y]);
				write(fd[1],&a[y],sizeof(a[y]));
				sleep(a[x]/3);

			}
		}


	}
}
