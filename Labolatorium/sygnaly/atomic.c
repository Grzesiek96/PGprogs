/* atomic.c.
 * Handlers that return normally must modify some global variable in order to have any effect.
 * Typically, the variable is one that is examined periodically by the program during normal 
 * operation. Its data type should be sig_atomic_t - because otherwise signal handler might 
 * be invoked in the middle of reading or writing the object.
 * The program below executes the body of the loop until it has noticed that a SIGALRM signal
 * has arrived. This technique is useful because it allows the iteration in progress when
 * the signal arrives to complete before the loop exits. 
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
    /* This flag controls termination of the main loop. */
volatile sig_atomic_t keep_going = 1;

void error(char *message)
{
	perror(message);
	kill(0, SIGTERM);
	exit(EXIT_FAILURE);
}

void setsighandler(int sig, void (*handler) (int))
{
	struct sigaction sa;

	memset(&sa, 0x00, sizeof(struct sigaction));
	sa.sa_handler = handler;
	if (sigaction(sig, &sa, NULL) == -1)
		error("sigaction");

}

/* The signal handler just clears the flag. */
void catch_alarm(int sig)
{
	keep_going = 0;
}

void do_stuff(void)
{

#define NR	2000000
	int cnt;

	unsigned long sum;

	printf("Doing stuff while waiting for alarm....");
	for (cnt = 0, sum = 0UL; cnt < NR; cnt++) {
		sum = sum + rand();
	}
	printf(" sum=%lu\n", sum);
}

int main(void)
{

	/* Establish a handler for SIGALRM signals. */
	setsighandler(SIGALRM, catch_alarm);

	/* Set an alarm to go off in a little while. */
	alarm(2);

	/* Check the flag once in a while to see when to quit. */
	while (keep_going)
		do_stuff();
	printf("Done\n");

	return EXIT_SUCCESS;
}
