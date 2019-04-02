#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


#define MAXPTHREADS	10
volatile int m, k;
volatile int keep_going =1;
volatile int flaga = 0;
//pthread_mutex_t blokada = PTHREAD_MUTEX_INITIALIZER;
//volatile int char_cnt[MAXPTHREADS]; //tablica liczników wystąpień liczb zakresu od 32 do 127
//volatile int cnt_loop =0;//counter wystąpień

sigset_t signalSet;//maska sygnałow


void *worker(void *arg)
{
	int sigNo;
	while (1) {
		//write(1, *(int*)arg, 1);
		if (sigwait(&signalSet, &sigNo)) {//czeka na sygnal i zwraca jego wartość gdy go otrzyma
			perror("sigwait");;
			break;
		}		
		if(sigNo== SIGUSR1){
			fprintf(stderr, "*n\n"); 
		}	
	}	
	
	pthread_exit((void *)0);	
		
}


int main(int argc, char *argv[])
{
	
	int retcode, n, i;
	pthread_t pthids[MAXPTHREADS];
	void *retval;
	sigfillset(&signalSet);
	sigdelset(&signalSet, SIGINT);
	pthread_sigmask(SIG_BLOCK, &signalSet, NULL);
	if (argc != 4){
		fprintf(stderr, "zla liczba argumentow"); 
		exit(1);
	}
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	k = atoi(argv[3]);	
	if (n < 1 || n > MAXPTHREADS || m<=0 || k<=0){
		fprintf(stderr, "zla wartosc argumentu"); 
		exit(2);		
	}
	
	for (i = 0; i < n; i++) {
		// printf("%d\n",n);
		retcode =
		    pthread_create(&(pthids[i]), NULL, worker, (void *)&i);//stworzenie wątku przekazanie argumentu n do wątku
		if (retcode != 0)
			fprintf(stderr, "create failed: n=%d, retcode=%d\n", n,
				retcode);
				
		fprintf(stderr, "pthids 1 %ld\n", pthids[i]);
			
						
	}
	/*for (i = 0; i < n; i++) {
		retcode = pthread_join(pthids[i], &retval);//czekanie dopóki wątek n dokończy pracę
		if (retcode != 0)
			fprintf(stderr, "join a failed %d\n", retcode);
		else
			fprintf(stderr, "join successful for n=%d\n", n);
	}*/
	
	while(1){
			sleep(1);
			for (i = 0; i < n; i++){ 
		//	fprintf(stderr, "pthids 2 %ld\n", pthids[i]);
			pthread_kill(pthids[i], SIGUSR1);
		}	
		
	}
	return 0;
}
