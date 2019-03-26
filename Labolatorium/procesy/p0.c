/* p0.c. Guess the output of this program before running */
#include <stdio.h>
#include <unistd.h> 
int main()
{
	printf("a");
	fork();
	printf("b");
	printf("\n");
	return 0;
}
