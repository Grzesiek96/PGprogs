/* hello_fork.c */
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
int main(void){
pid_t pid;

	printf("hello_fork! (PID=%lu)\n",(unsigned long)getpid());
	pid=fork();
	switch(pid){
		case -1: perror("fork"); return 1;
		case 0: printf("potomek: PID=%lu\n",(unsigned long)getpid());
		sleep(5);
		printf("potomek kończy\n");
		return 0;
		default: printf("rodzic: PID=%lu\n\n",(unsigned long)getpid()); 
		if(wait(NULL)<0) perror("wait");
		printf("KONIEC\n");
		return 0;
	}
}
