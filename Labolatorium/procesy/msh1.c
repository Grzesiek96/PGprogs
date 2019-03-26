/* msh1.c - a simple command interpreter.
 * There are 2 internal commands: times, quit.
 * System shell is used for execution of external commands.
 * Note: commands input can be redirected to a file, e.g.
 * 	./msh1 < commands_file
 * if commands_file contains commands to interpret.
 * Also stdout and stderr can be redirected ...
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/times.h>

void print_times(FILE * out)
{
	clock_t clock;
	double cstime, cutime, stime, utime;
	struct tms tmsbuf;
	clock = times(&tmsbuf);
	if(clock==(clock_t)-1){
		perror("clock");
		return;
	}
	stime = ((double)tmsbuf.tms_stime) / (double)CLOCKS_PER_SEC *1e3;
	utime = ((double)tmsbuf.tms_utime) / (double)CLOCKS_PER_SEC *1e3;
	cstime = ((double)tmsbuf.tms_cstime) / (double)CLOCKS_PER_SEC *1e3;
	cutime = ((double)tmsbuf.tms_cutime) / (double)CLOCKS_PER_SEC *1e3;
	if (out == NULL)
		out = stderr;
	fprintf(out, "\nParent   system mode time  = %10.3f ms\n", stime);
	fprintf(out, "Parent   user   mode time  = %10.3f ms\n", utime);
	fprintf(out, "Children system mode time  = %10.3f ms\n", cstime);
	fprintf(out, "Children user   mode time  = %10.3f ms\n", cutime);
	return;
}

int internal_cmd(char *cmd)
{
	if (strncmp(cmd, "times", 6) == 0) {	/* display time usage */
		print_times(stdout);
		return 1;
	} else if (strncmp(cmd, "quit", 5) == 0) {	/* exit the interpreter */
		exit(0);
	}
	return 0;
}

int external_cmd(char *cmd)
{
	int status;
	if (cmd == NULL) {	/* Check if a system shell is available */
		status = system(NULL);
		if (status <= 0) {
			fprintf(stderr, "system shell is not available\n");
			return 1;
		} else {
			return 0;
		}
	}
	fflush(NULL);		/* flush output buffers */
	status = system(cmd);	/* execute the command in buffer by shell */
	if (status == -1) {
		perror("system");
		return -1;
	}
	if (WIFEXITED(status)) {
		fprintf(stderr, "Process exited with code=%d\n",
			WEXITSTATUS(status));
	}
	if (WIFSIGNALED(status)) {
		fprintf(stderr, "Process got signal nr %d\n", WTERMSIG(status));
	}
	if (WIFSTOPPED(status)) {
		fprintf(stderr, "Process stopped by signal nr %d\n",
			WSTOPSIG(status));
	}
	return status;
}

int main(int argc, char *argv[])
{
	char buf[BUFSIZ];
	int status;

	while (1) {
		int cmdlen;
		if (isatty(STDIN_FILENO))
			printf("\n> ");	/* Display prompt if input comes from terminal */
		fflush(NULL);
		if (fgets(buf, sizeof(buf), stdin) == NULL)
			break;
		cmdlen = strlen(buf);
		if (buf[cmdlen - 1] == '\n')
			buf[cmdlen - 1] = '\0';
		if (buf[0] == '\0')
			continue;
		if (!isatty(STDIN_FILENO))	/* echo command if not input from terminal */
			printf("#cmd:# %s\n", buf);
		/* check for internal commands */
		if (internal_cmd(buf))
			continue;
		/* external commands are processed with a system shell */
		status = external_cmd(buf);
		fprintf(stderr,"\nstatus=%d\n",status);
	}
	if (ferror(stdin)) {
		fprintf(stderr, "stdin error\n");
		return 1;
	}
	print_times(NULL);
	return 0;
}
