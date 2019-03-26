/* p3.c. Guess the number of processes and the output of the program
 * before running it.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
	int i, imax;
	if (argc < 2)
		imax = 2;
	else
		imax = atoi(argv[1]);
	printf("imax=%d, \n", imax);
	for (i = 0; i < imax; i++) {
		printf("a");
		if (fork() < 0)
			return 1;
		printf("b");
	}
	printf("\n");
	return 0;
}
