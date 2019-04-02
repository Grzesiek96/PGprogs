#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
volatile sig_atomic_t keep_going = 1, flaga=0;

sigset_t signalSet;
pthread_t th_a;

/*void setsighandler(int sig, void (*handler) (int))
{
	struct sigaction sa;

	memset(&sa, 0x00, sizeof(struct sigaction));
	sa.sa_handler = handler;
	if (sigaction(sig, &sa, NULL) == -1){
		perror("sigaction"); exit(EXIT_FAILURE);
	}
}

void handler(int sig)
{	
	if(sig==SIGALRM){ 
		//fprintf(stderr,"\n===> handler: sig=%d\n",sig);
		pthread_kill(th_a, SIGUSR1);
	}
	if(sig==SIGUSR1){
		flaga=1;
	}
}*/


void *worker(void *arg)
{
	//setsighandler(SIGUSR1, handler);
	int sigNo;
	while (1) {
		write(1, (int*)arg, 1);
		if (sigwait(&signalSet, &sigNo)) {//czeka na sygnal i zwraca jego wartość gdy go otrzyma
			perror("sigwait");;
			break;
		}		
		fprintf(stderr, "sigNo=%d, tid=%lu\n", sigNo,
			(unsigned long)pthread_self());
		if(flaga){
			write(1, "*", 1);
			fprintf(stderr, "dxxx"); 
			flaga=0;
		}	
		if(sigNo == SIGINT)
			keep_going = 0;
	}
	//return NULL;
	pthread_exit((void *)0);
}





int main(void)
{
	int retcode;
	
	
	void *retval;
	//setsighandler(SIGALRM, handler);	
	sigfillset(&signalSet);
	//sigdelset(&signalSet, SIGALRM);
	sigdelset(&signalSet, SIGUSR1);
	sigdelset(&signalSet, SIGINT);
	/*
	 * Block signals in initial thread. New threads will
	 * inherit this signal mask.
	 */
	pthread_sigmask(SIG_BLOCK, &signalSet, NULL);
	
	retcode = pthread_create(&th_a, NULL, worker, (void*)&i);
	if (retcode != 0)
		
	retcode = pthread_join(th_a, &retval);
	if (retcode != 0)
		fprintf(stderr, "pthread_join error (a): %d\n", retcode);
	while(keep_going){
		write(1, ".", 1);	
		sleep(1);
		pthread_kill(th_a, SIGALRM);
	}
	printf("KONIEC");
	return 0;
}
