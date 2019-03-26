/* sig2.c. Example of sigaction(), sigprocmask() use.
 * Illustration of side-effects of signal handling on sleep() execution.
 */
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

volatile sig_atomic_t do_exit, next_phase;

void setsighandler(int sig, void (*handler) (int))
{
	struct sigaction sa;

	memset(&sa, 0x00, sizeof(struct sigaction));
	sa.sa_handler = handler;
	if (sigaction(sig, &sa, NULL) == -1){
		perror("sigaction"); exit(EXIT_FAILURE);
	}
}

void handler(int sig)
{
	fprintf(stderr,"\n===> handler: sig=%2d\n",sig);
	if(sig==SIGALRM) do_exit++;
	if(sig==SIGTSTP) next_phase++;
	sleep(1); /* give the time for another signal to arrive */
	fprintf(stderr,"\n<=== return from handler(%2d), do_exit=%2d,next_phase=%2d\n",
		sig,do_exit,next_phase);
}

int main(int argc, char *argv[])
{
	int t;
	sigset_t oldset, set;
/*
 *	Install SIGINT, SIGTSTP SIGALRM signal handlers
 */
	setsighandler(SIGINT, handler); /* ^C */
	setsighandler(SIGTSTP, handler);/* ^Z */
	setsighandler(SIGALRM, handler);

	fprintf(stderr,"%s: PID=%lu\n", argv[0], (unsigned long)getpid());
	fprintf(stderr,"\nSend SIGINT singnals with ^C\n");
	fprintf(stderr,"Go to the next program phase by sending SIGTSTP signal (^Z)\n\n");
	alarm(60); /* generate SIGALRM in 60 seconds */
	while(!do_exit && !next_phase) {
		t=5;
		fprintf(stderr,"*** sleeping for %d seconds\n",t);
		while(t>0)
		{
			fprintf(stderr,"sleep(%d)\n",t);
			t=sleep(t);
			fprintf(stderr,"       t=%d\n",t);
		}
		fprintf(stderr,"*** sleeping completed\n");
	}
	sigfillset(&set);
	if(sigprocmask(SIG_BLOCK,&set,&oldset)<0){
		perror("sigprocmask1"); return 1;
	}
	next_phase=0;
	fprintf(stderr,"\nNow signals will be ignored for 10 seconds (try to imterrupt :)\n");
	fprintf(stderr,"*** sleeping for 10 seconds\n");
	t=sleep(10);
	fprintf(stderr,"\n*** sleep(10) returned %d\n",t);
	fprintf(stderr,"\nNow the previous signal handling is restored\n");
	if(sigprocmask(SIG_SETMASK,&oldset,NULL)<0){
		perror("sigprocmask2"); return 2;
	}
	while(!do_exit && !next_phase) {
		t=5;
		fprintf(stderr,"*** sleeping for %d seconds\n",t);
		while(t>0)
		{
			fprintf(stderr,"sleep(%d)\n",t);
			t=sleep(t);
			fprintf(stderr,"       t=%d\n",t);
		}
		fprintf(stderr,"*** sleeping completed\n");
	}
	
	fprintf(stderr,"Enough is enough. Exiting...\n");
	return 0;
}
