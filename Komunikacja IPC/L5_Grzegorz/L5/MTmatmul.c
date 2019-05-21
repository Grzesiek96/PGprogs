/* MTmatmul.c -multithreaded matrix multiplication for a multiprocessor system */
//liczba wątków obliczających to parametr argv[1]

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <sys/times.h>
/*
 * condition variable function prototypes
 */

#define SZ 400

struct {
	pthread_mutex_t lock;
	pthread_cond_t start_cond, done_cond;
	double (*m1)[SZ][SZ], (*m2)[SZ][SZ], (*m3)[SZ][SZ];
	int row, col;
	int todo, notdone, workers;
} work;

pthread_mutex_t mul_lock;
pthread_t tid;

int NTHR;
static void matmul(double (*m1)[SZ][SZ], double (*m2)[SZ][SZ],
		   double (*m3)[SZ][SZ]);
static void *worker(void *);

int main(int argc, char *argv[])
{

	int row, col, CLK_TCK, NCPU;
	double m1[SZ][SZ], m2[SZ][SZ], m3[SZ][SZ];
	clock_t start, end, clk1, clk2;
	double tm2;
	struct tms tms1, tms2;
	if (argc == 2)
		NTHR = atoi(argv[1]);
	else
		NTHR = 2;

	NCPU = (int)sysconf(_SC_NPROCESSORS_ONLN);
	CLK_TCK = sysconf(_SC_CLK_TCK);
	printf("Nr of threads=%d, nr of processors online=%d\n", NTHR, NCPU);
	printf("Size of the system of equations: %d\n", SZ);
	
	//przypisanie zmiennych losowych do tabel
	for (row = 0; row < SZ; row++)
	{
		for (col = 0; col < SZ; col++)
		{
			m1[row][col] = rand();
			m2[row][col] = rand();
		}
	}
	//zainicjowanie sekcji krytycznej
	pthread_mutex_init(&mul_lock, NULL);
	pthread_mutex_init(&work.lock, NULL);
	start = clock();
	clk1 = times(&tms1);
	//wywolanie funkcji generującej wątki o liczbie argv[i]
	matmul(&m1, &m2, &m3);
	
	clk2 = times(&tms2);
	end = clock();

	tm2 = (double)(end - start) / CLOCKS_PER_SEC;
	printf("\nThe time with MT was: %f\n", tm2);
	printf("Elapsed     time was: %f sec.\n",
	       (double)(clk2 - clk1) / CLK_TCK);
	printf("User   mode time was: %f sec.\n",
	       (double)(tms2.tms_utime - tms1.tms_utime) / CLK_TCK);
	printf("System mode time was: %f sec.\n",
	       (double)(tms2.tms_stime - tms1.tms_stime) / CLK_TCK);
	fflush(stdout);
	pthread_mutex_destroy(&work.lock);
	pthread_mutex_destroy(&mul_lock);

	return (0);
}

static void matmul(double (*m1)[SZ][SZ], double (*m2)[SZ][SZ],
		   double (*m3)[SZ][SZ])
{
	int i;
	//zablokowanie sekcji krytycznej
	pthread_mutex_lock(&mul_lock);
	pthread_mutex_lock(&work.lock);

	if (work.workers == 0) {//jeśli pierwsze wywołanie
		for (i = 0; i < NTHR; i++) {
			pthread_t tid;
			if (pthread_create(&tid, NULL, worker, NULL) != 0) {
				fprintf(stderr, "pthread_create failed\n");
				return;
			}
		}
		work.workers = NTHR;// numer wątku (nie mylić z ID)
	}
	//wpisanie wartości wygenerowanych do poszczegolnych tabel
	work.m1 = m1;
	work.m2 = m2;
	work.m3 = m3;
	work.row = work.col = 0;
	work.todo = work.notdone = SZ * SZ;//400*400

	pthread_cond_broadcast(&work.start_cond);//wznowienie wszystkich wątków oczekujących na zmiennej

	while (work.notdone)
		pthread_cond_wait(&work.done_cond, &work.lock);//zwolnienie mutexa i wstrzymanie wątka w oczekiwaniu na spełnienie warunku

	pthread_mutex_unlock(&mul_lock);
	pthread_mutex_unlock(&work.lock);
}

static void *worker(void *arg)
{
	double (*m1)[SZ][SZ], (*m2)[SZ][SZ], (*m3)[SZ][SZ];
	int row, col, i;
	double result;

	while (1) {
		pthread_mutex_lock(&work.lock);
		while (work.todo == 0)//wykona się tylko wtedy kiedy todo będzie zero
			pthread_cond_wait(&work.start_cond, &work.lock);//czekanie aż będzie spełniony warunek start cond dla mutexa work.lock
			//zatrzymanie wątka aż do spełnienia warunku

		work.todo--;

		m1 = work.m1;
		m2 = work.m2;
		m3 = work.m3;
		row = work.row;
		col = work.col;
		work.col++;
		if (work.col == SZ)//jesli kolumny dojda do 400
		{
			work.col = 0;//zwieksz wiersz
			work.row++;
			if (work.row == SZ)//jesli wiersze
				work.row = 0;
		}
		pthread_mutex_unlock(&work.lock);//zwolnienie sekcji krytycznej
		result = 0;
		for (i = 0; i < SZ; i++)
			result += (*m1)[row][i] * (*m2)[i][col];

		(*m3)[row][col] = result;

		pthread_mutex_lock(&work.lock);
		work.notdone--;
		if (work.notdone == 0)
			pthread_cond_signal(&work.done_cond);//wznowienie jednego z wątków

		pthread_mutex_unlock(&work.lock);
	}
}
