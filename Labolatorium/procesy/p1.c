/* p1.c. Guess the output of this program before running */
#include <stdio.h>
#include <unistd.h>
int main()
{
	fprintf(stderr, "a");
	fork();
	fprintf(stderr, "b");
	return 0;
}
