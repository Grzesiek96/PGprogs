#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

//=====================deklaracja zmiennych
#define MAXPTHREADS	10

volatile int keep_going =1; 
volatile sig_atomic_t INT_flaga, QUIT_flaga, USR1_flaga;
volatile int n, m, k;
pthread_mutex_t blokada = PTHREAD_MUTEX_INITIALIZER;
//volatile int char_cnt[MAXPTHREADS]; //tablica liczników wystąpień liczb zakresu od 32 do 127
volatile int cnt_counter =0;//counter wystąpień poszczegol

sigset_t signalSet;//maska sygnałow
pthread_t pthids[MAXPTHREADS];
//===========================

//===============handler sygnalow
void setsighandler(int sig, void (*handler) (int))
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
	if(sig==SIGUSR1){ 
		//fprintf(stderr,"===> SIGUSR1: sig=%d\n",sig);
		USR1_flaga = 1;	
	}
	if(sig==SIGQUIT){ 
		//fprintf(stderr,"===> SIGUSR1: sig=%d\n",sig);
		QUIT_flaga = 1;	
	}
	if(sig==SIGINT){ 
		//fprintf(stderr,"===> SIGUSR1: sig=%d\n",sig);
		INT_flaga = 1;	
	}	
}


void *worker(void *arg)
{
	int sigNo, counter, sig;//numer sygnalu oraz lokalna wartosc watku
	//deklaracja lokalnych counterow sygnalow SIGQUIT oraz SIGINT
	int sigint_cnt, sigquit_cnt;
	//================pobranie argumentu	
	sig = *((int *)arg);
	
	while (1) {
		setsighandler(SIGUSR1, handler);//oczekiwanie na sygnal SIGUSR1
		setsighandler(SIGINT, handler);//oczekiwanie na sygnal SIGINT
		setsighandler(SIGQUIT, handler);//oczekiwanie na sygnal SIGQUIT
		
		if(USR1_flaga){//wykonuje się kiedy watek wysle sygnal SIGUSR1
			fprintf(stderr,"SIGUSR1: current thread =%lu number =%d \n",(unsigned long)pthread_self(), sig);
			USR1_flaga = 0;
		}
		if(QUIT_flaga){//wykonuje się kiedy watek wysle sygnal SIGUSR1
			fprintf(stderr,"SIGQUIT1: current thread =%lu number =%d \n",(unsigned long)pthread_self(), sig);
			sigquit_cnt++;
			QUIT_flaga = 0;
		}
		if(INT_flaga){//wykonuje się kiedy watek wysle sygnal SIGUSR1
			fprintf(stderr,"SIGINT: current thread tid =%lu  number =%d \n",(unsigned long)pthread_self(), sig);
			sigint_cnt++;
			INT_flaga = 0;
		}
		if (sigwait(&signalSet, &sigNo)) {//czeka na sygnal i zwraca jego wartość gdy go otrzyma
			perror("sigwait");;
			break;
		}		
		if(sigNo== SIGALRM){
			sleep(m); //watek spi przez m sekund
			//==========wyslanie znaku ASCII===========
			int znak = 40 + sig;
			fprintf(stderr, "Znak %c \n", znak);
			//============zwiekszanie globalnego licznika
			counter++;
			pthread_mutex_lock(&blokada);
			cnt_counter = counter;
			pthread_mutex_unlock(&blokada);
			//========================
			//===========wyslanie sygnalu start do losowego watku
			pthread_kill(pthids[rand()%n], SIGUSR1);			
		}
		//warunek zakończenia pracy watków pobocznych	
		if(cnt_counter == k){
			fprintf(stderr,"Watek zakonczono =%lu, sigint_cnt =%d sigquit_cnt=%d\n",
	(unsigned long)pthread_self(),sigint_cnt, sigquit_cnt);
			break;
		}	
		
}
//wydruk wynikow na standardowe wyjsce
	
	keep_going =0; // przerwanie obiegu petli w glownym watku
	pthread_exit((void *)0);//konczenie watku	
		
}


int main(int argc, char *argv[])
{
	
	int retcode, i;
	int dot_counter=0;//licznik
	
	void *retval;
	//======================ustawienie maski sygnałow
	sigfillset(&signalSet);
	sigdelset(&signalSet, SIGINT);
	sigdelset(&signalSet, SIGQUIT);
	sigdelset(&signalSet, SIGUSR1);
	//==================
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
			fprintf(stderr, "create failed: n=%d, retcode=%d\n", n, retcode);
				
		//fprintf(stderr, "pthids 1 %ld\n", pthids[i]);
	}

	while(keep_going){
			//sleep(1);
			write(1, ".", 1);
			dot_counter++;
			for (i = 0; i < n; i++){ 
			pthread_kill(pthids[i], SIGALRM);
		}	
		fprintf(stderr, "Licznik wywolania watkow %d \n", cnt_counter);
		
	}
	printf("Licznik kropek%d \n", dot_counter);
	return 0;
}
