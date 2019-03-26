/* hello_fork2.c */
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
int main(void){
pid_t pid;
int v;
int *status=&v;

	printf("hello_fork!\n");
	pid=fork();
	switch(pid){
		case -1: perror("fork"); return 1;
		case 0: 
			printf("child PID=%ld\n",(long)getpid());
			execl("/bin/ls","ls","-l",NULL); 
			perror("exec");
			return 2;
		default: 
			printf("parent PID=%ld\n",(long)getpid()); 
			sleep(10);
			if(wait(status)<0){
				perror("wait");
				return 1;
			}
			printf("status=%d\n",*status);
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
