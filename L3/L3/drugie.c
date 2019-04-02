#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>


#define MAXPTHREADS	10
//#define RAND_MAX 100
//pthread_mutex_t blokada = PTHREAD_MUTEX_INITIALIZER;
volatile int char_cnt[MAXPTHREADS]; //tablica liczników wystąpień liczb zakresu od 32 do 127
volatile int cnt_loop =0;//counter wystąpień

void *worker(void *arg)
{
	int c=0;
	int b=0;//b- przechowuje ilosc bajtow dotychczas zapisanych
	int x= ((int *)arg)[0];//przypisanie argumentu arg z rzutowaniem
	fprintf(stderr, "wartosc n %d\n", x);
	while((c = getchar()) != EOF){
			if(b == 256 || c == '\n'){
				fprintf(stderr, "Wartosc cnt loop %d\n", cnt_loop);
				char_cnt[x] = cnt_loop;//w
				uint r = (rand()%100);
				fprintf(stderr, "Spanie %u \n", r);
				//sleep(r);
				break;
			}
			else{
				b++;
				if(c >=32 && c <=127){
					cnt_loop++;
				}
			}
			
	}
	
	
}


int main(int argc, char *argv[])
{
	int retcode, n, i;
	pthread_t pthids[MAXPTHREADS];
	void *retval;
	if (argc != 2){
		printf("zla liczba argumentow"); 
		exit(1);
	}
	n = atoi(argv[1]);
	if (n < 1 || n > MAXPTHREADS){
		printf("za duza wartosc argumentu"); 
		exit(2);		
	}
	
	for (i = 0; i < n; i++) {
		// printf("%d\n",n);
		retcode =
		    pthread_create(&(pthids[i]), NULL, worker, (void *)&i);//stworzenie wątku przekazanie argumentu n do wątku
		if (retcode != 0)
			fprintf(stderr, "create failed: n=%d, retcode=%d\n", n,
				retcode);
	}
	
	for (i = 0; i < n; i++) {
		retcode = pthread_join(pthids[i], &retval);//czekanie dopóki wątek n dokończy pracę
		if (retcode != 0)
			fprintf(stderr, "join a failed %d\n", retcode);
		else
			fprintf(stderr, "join successful for n=%d\n", n);
	}
	for(i =0; i<n; i++){
		printf("Ilosc licznikow tabeli %d= %d\n", i, char_cnt[i]);
	}
	return 0;
}
