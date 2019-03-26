/* sig1.c. The program is based on the example 2.4.10 from the book:
 * S.A. Rago, "UNIX System V Netowork Programming".
 * One can see, that using signal() (at least when implemented traditionally)
 * is risky, because kernel resets signal handling to default before
 * actually starting execution of the previously defined signal handler.
 * To keep signal handling it is necessary to define it again - by a call
 * to signal() in the handler.
 * Note, that the signal handler function in this program creates some
 * extra delay - before the handler is re-defined. Thus it is not difficult
 * to send in this time interval several signals SIGINT (e.g. ^C from
 * the keyboard). One can see that behaviour, when SIGINT signal is
 * delivered while the handler executes sleep().
 * In modern Linux systems signal() does not reinstalls default signal
 * disposition. In these systems it is possible to get the traditional
 * behaviour, eg. by using sysv_signal() function instead of signal().
 * (uncomment #define OLD)
 */
//#define OLD
#ifdef OLD
#define _GNU_SOURCE
#define	SIGNAL	sysv_signal
#else
#define SIGNAL	signal
#endif
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void handler(int sig)
{
	printf("SIGINT handler\n");
/*
 * Wait a moment, increasing a chance for the SIGINT signal to be delivered
 * before re-definition of SIGINT signal handler.
 */
	sleep(1);

/*
 *	Re-define SIGINT signal handler
 */
	SIGNAL(SIGINT, handler);

}

int main(int argc, char *argv[])
{
	int i;

/*
 *	Install SIGINT signal handler
 */
	SIGNAL(SIGINT, handler);

	printf("%s: PID=%lu\n", argv[0], (unsigned long)getpid());

/*
 *	Call pause() 10 times, waiting each time for end of signal handling.
 */
	for (i = 0; i < 10; i++) {
		printf("waiting for a signal\n");
		pause();
		printf("got a signal\n");
	}

	return 0;
}
