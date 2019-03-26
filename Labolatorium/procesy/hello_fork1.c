/* hello_fork1.c */
#include <errno.h>
#include <unistd.h>
//#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
pid_t pid;
int signr=0, v;
int *pstatus=&v;

	printf("hello_fork!\n");
	if(argc>1) signr=atoi(argv[1]);
	pid=fork();
	switch(pid){
		case -1: perror("fork"); return 1;
		case 0: 
			printf("child PID=%ld\n",(long)getpid()); 
			if(signr) kill(getpid(),signr); 
			sleep(5); 
			return 100;
		default: printf("parent PID=%ld\n",(long)getpid()); 
			sleep(10);
			if(wait(pstatus)<0){
				perror("wait");
				return 1;
			}
			printf("child status=%d\n",*pstatus);
			if(WIFEXITED(v)){
				printf("child exited with %d\n",WEXITSTATUS(v));
			}
			if(WIFSIGNALED(v)){
				printf("child terminated with signal: %d\n",WTERMSIG(v));
			}
			sleep(10);
			return 0;
	}
}
