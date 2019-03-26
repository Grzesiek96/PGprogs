// Użycie: W jednym terminalu skompiluj i uruchom program
// W drugim terminalu wpisz $ps, aby zobaczyć PID tego 
// programu z pierwszego terminala. Aby wysłać do niego
// sygnał wpisz w terminalu drugim $kill -SIGUSR1 PID
// albo SIGUSR2 i obserwuj jak się zmienia wartosć w 
// terminalu pierwszym. Aby zakończyć - wciśnij <CTRL>C
// w pierwszym terminalu i odczekaj parę s...

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define DECREASE -1
#define HOLD 0
#define INCREASE 1

volatile sig_atomic_t keep_going = 1;
volatile sig_atomic_t change_char = 0;

void handler(int sign){
	if(sign == SIGUSR1)
		change_char = INCREASE;
	else if(sign == SIGUSR2)
		change_char = DECREASE;
	else
		keep_going = 0;
}

void error(char *msg){
	perror(msg);
	fprintf(stderr, "Error number: %d", errno);
	// kills every process of this PGID
	// i.e. parent and it's children (if any exist)
	kill(0, SIGTERM); 
	exit(EXIT_FAILURE);
}

void setSigHandler(int sig_num, void (*handler)(int)){
	struct sigaction sa;
	// initializes struct sa with 0x00
	memset(&sa, '\0', sizeof(struct sigaction)); 

	sa.sa_handler = handler;
	// if sigaction is not successful, it returns -1
	if(sigaction(sig_num, &sa, NULL) == - 1)
		error("sigaction");
}

void changeChar(char *ch, int change){
	if((*ch + change) == 'z' + INCREASE)
		*ch = '0';
	else if((*ch + change) == '0' + DECREASE)
		*ch = 'z';
	else
		*ch += change;
}


int main(void){
	setSigHandler(SIGUSR1, handler);
	setSigHandler(SIGUSR2, handler);
	setSigHandler(SIGINT, handler);
	char ch = '0';

	while(keep_going){
		if(change_char == INCREASE){
			changeChar(&ch, INCREASE);
			change_char = HOLD;
		}
		else if(change_char == DECREASE){
			changeChar(&ch, DECREASE);
			change_char = HOLD;
		}
		printf("%c\n", ch);
		usleep(1500000L);
	}
	printf("\nTHE END\n");
	sleep(5);

	return 0;
}








