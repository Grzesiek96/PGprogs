#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#define SHM_KEY 10
#define MSG_KEY 20
#define SHM_SIZE 10

// Kompilacja gcc -o com com.c
// Wywołanie ./com [polecenie]
// polecenie = {rejestruj, wyrejestruj, odczytaj, wyslij, odbierz}

void rejestruj();
void wyrejestruj();
void odczytaj();
void wyslij();
void odbierz();

void (*operacje[5])();

int main(int argc, char** argv)
{
	int iter;
	char *polecenia[] = {"rejestruj", 
						 "wyrejestruj",
						 "odczytaj",
						 "wyslij",
						 "odbierz"};
	char *polecenie;
	
	operacje[0] = rejestruj;
	operacje[1] = wyrejestruj;
	operacje[2] = odczytaj;
	operacje[3] = wyslij;
	operacje[4] = odbierz;	
	
	if(argc != 2)
	{
		perror("Blad wywolania - niepoprawna liczba argumentow");
		exit(1);
	}
	
	polecenie = argv[1];
	for(iter=0; iter<5; iter++)
	{
		if(strcmp(polecenie, polecenia[iter]) == 0)
		{
			(*operacje[iter])(); // Wybranie stosownego polecenia
		}
	}
	
	return 0;
}
// Polecenia
void rejestruj()
// Zapisanie identyfikatora procesu w  w pamięci dzielonej
{ 
	char *adres;
	char  pidStr[12];
	pid_t pid;
	pid = getpid();
	sprintf(pidStr, "%d", (int)pid );	// Konwersja pid do string
	
	
	int shmflg, shmid;
	shmflg = 0;
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, shmflg)) == -1)
	{
		perror("Blad shmget\n");
		exit(1);
	}
	
	// Pobranie adresu
	if ((adres = shmat(shmid, 0, shmflg)) == -1)
	{
		perror("shmat");
		exit(1);
	}
	
	strcpy(adres, pidStr);
	printf("Zapisano pid\n");
	
	if (shmdt(adres) == -1)
	{
		perror("shmdt");
		exit(1);
	}
	
}
void wyrejestruj()
{
	char *adres;
	char *pidStr[12];
	
	int shmflg, shmid;
	shmflg = 0;
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, shmflg)) == -1)
	{
		perror("Blad shmget\n");
		exit(1);
	}
	
	// Pobranie adresu
	if ((adres = shmat(shmid, 0, shmflg)) == -1)
	{
		perror("shmat");
		exit(1);
	}
	strcpy(pidStr, adres);
	printf("Usunieto pid danego procesu: %s", pidStr);
}
void odczytaj()
// Odczyt zarejestrowanych pidów w pamięci
{
	char *adres;
	char *pidStr[12];
	
	int shmflg, shmid;
	shmflg = 0;
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, shmflg)) == -1)
	{
		perror("Blad shmget\n");
		exit(1);
	}
	
	// Pobranie adresu
	if ((adres = shmat(shmid, 0, shmflg)) == -1)
	{
		perror("shmat");
		exit(1);
	}
	strcpy(pidStr, adres);
	printf("Odcztano pid: %s", pidStr);
	
}
void wyslij()
{
	printf("Funkcja wyslij\n");
}
void odbierz()
{
	printf("Funkcja odbierz\n");
}

