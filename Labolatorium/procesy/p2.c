/* p2.c. Try to observe (e.g. with "ps -ef |grep p2" command) processes
 * created with this program for different values of arguments, e.g
 * ./p2
 * ./p2 5 10
 * Pay attention to orphans and zombies.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
	int tp = 15, tc = 10;
	pid_t pid;
	if (argc > 1)
		tp = atoi(argv[1]);
	if (argc > 2)
		tc = atoi(argv[2]);
	printf("process %lu starts sleeping for 5 secs\n", (unsigned long)getpid());
	sleep(5);
	printf("forking\n");
	switch (pid = fork()) {
	case -1:
		perror("fork");
		return 1;
		break;
	case 0:
		printf("child %lu sleeping for %d secs\n", (unsigned long)getpid(), tc);
		sleep(tc);
		printf("child exiting\n");
		break;
	default:
		printf("parent sleeping for %d secs\n", tp);
		sleep(tp);
		printf("parent exiting\n");
		break;
	}
	return 0;
}
