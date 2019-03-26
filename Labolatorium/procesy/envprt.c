/* envprt.c.
 * Displays values of environment variables, listed as arguments of the program.
 * If the program is executed without arguments - names of the variables
 * are read from the standard output (one name per line).
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
	char buf[BUFSIZ], *name, *p;
	int n = 1;
	while (1) {
		if (argc > 1) {
			if (n >= argc)
				break;
			else
				p = getenv(argv[n]);
			name = argv[n];
			n++;
		} else {
			if (fgets(buf, sizeof(buf), stdin) == NULL)
				break;
			name = strtok(buf, " \t\n");
			if (!name)
				continue;
			else
				p = getenv(name);
		}
		if (p == NULL) {
			printf("%s is undefined\n", name);
		} else {
			printf("%s=%s\n", name, p);
		}
	}
	return argc - 1;
}
