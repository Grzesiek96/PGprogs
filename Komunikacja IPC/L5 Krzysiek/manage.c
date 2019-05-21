#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#define shmKey 		1959
#define msgKey 		1996
#define SHM_SIZE 	10
// Kompilowanie: gcc -o manage manage.c
// Wywołanie: manage dodaj lub manage usun

int main(int argc, char** argv)
{
	char *polecenie, *polDodaj, *polUsun;

	int msgflg, msgid, shmflg, shmid;
	if(argc != 2)
	{
		perror("Blad wywolania - niepoprawna liczba argumentow");
		exit(1);
	}
	polecenie = argv[1];
	polDodaj = "dodaj";
	polUsun = "usun";
	if(strcmp(polecenie, polDodaj) == 0)
	{
		
		msgflg = 0666|IPC_CREAT;
		
		// Utworzenie kolejki komunikatów
		if( (msgid = msgget(msgKey, msgflg)) == -1)
		{
			perror("Blad msgget.\n");
			exit(1);
		}
		
		shmflg = 0666|IPC_CREAT;
		
		// Utworzenie pamięci dzielonej
		if( (shmid = shmget(shmKey, SHM_SIZE, shmflg)) == -1)
		{
			perror("Blad shmget\n");
			exit(1);
		}
		printf("shmget returned %d", shmid);
		printf("Utworzono pamiec dzielona oraz kolejke komunikatow.\n");
		exit(0);
	}
	else if(strcmp(polecenie, polUsun) == 0)
	{
		// Usuniecie kolejki komunikatow
		if( (msgctl(msgid, IPC_RMID, 0)) == -1)
		{
			perror("Blad usuniecia kolejki komunikatow\n");
			exit(1);
		}
		
		// Usuniecie pamięci dzielonej
		if( (shmctl(shmid, IPC_RMID, 0)) == -1)
		{
			perror("Blad usuniecia pamieci dzielonej\n");
		}
		
		
		printf("Usunieto pamiec dzielona oraz kolejke komunikatow.\n");
		exit(0);
	}
	else
	{
		perror("Niepoprawny argument.\n");
		exit(1);
	}
	
	return 0;
}
