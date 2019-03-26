
/* sig1act.c. Modification of sig1.c which uses sigaction()
 */
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
void sethandler(int sig, void (*f) (int))
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = f;
	if (sigaction(sig, &sa, NULL) == -1){
		perror("sigaction:");
		exit(1);
	}
}

void handler(int sig)
{
	printf("SIGINT handler\n");
/*
 * Wait a moment, increasing a chance for the SIGINT signal to be delivered
 * before returning from this signal handler.
 */
	sleep(1);

}

int main(int argc, char *argv[])
{
	int i;
/*
 *	Install SIGINT signal handler
 */
	sethandler(SIGINT, handler);

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
