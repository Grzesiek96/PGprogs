#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
volatile sig_atomic_t keep_going = 1;

sigset_t signalSet;

void *worker(void *arg)
{
	int i = ((int *)arg)[0];
	int sigNo;
	while (1) {
		write(1, (int*)arg, 1);
		if (sigwait(&signalSet, &sigNo)) {//czeka na sygnal i zwraca jego wartość gdy go otrzyma
			perror("sigwait");;
			break;
		}
		
		fprintf(stderr, "sigNo=%d, tid=%lu, number of n=%d\n", sigNo,
			(unsigned long)pthread_self(), i);
		if(sigNo == SIGINT)
			keep_going = 0;
	}
	pthread_exit((void *)0);
}




#define MAXPTHREADS	10
int main(int argc, char* argv[])
{
	int retcode, n;
	pthread_t pthids[MAXPTHREADS];
	
	void *retval;
	//assert(sizeof(int) <= sizeof(void *));
	if (argc != 2)
		printf("Zla liczba arg"); exit(1);
	n = atoi(argv[1]);
	if (n < 1 || n > MAXPTHREADS)
		printf("Zle wartosc arg"); exit(2);
		
	
	sigfillset(&signalSet);
	/*
	 * Block signals in initial thread. New threads will
	 * inherit this signal mask.
	 */
	pthread_sigmask(SIG_BLOCK, &signalSet, NULL);
	int i;
	for (i = 0; i < n; i++) {
		retcode =
		    pthread_create(&(pthids[i]), NULL, worker, (void*)&n );
	if (retcode != 0)
		fprintf(stderr, "pthread_join error (a): %d\n", retcode);
	}	
	//retcode = pthread_join(th_a, &retval);
	//if (retcode != 0)
	//	fprintf(stderr, "pthread_join error (a): %d\n", retcode);
	while(keep_going){
		write(1, ".", 1);	
		sleep(1);
	}
	
	return 0;
}
