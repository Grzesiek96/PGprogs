#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

// WYWOLANIE: ./szukaj "Ala" "tempFolder" 
int main(int argc, char *argv[])
{
	char *katalog, *wyraz, *filePath;
	DIR *dir;
	struct dirent *pozycja;
	
	int fileCounter;
	int fd;
	
	void *adres;
	struct stat fileStat;
	int fileSize;
	int i,j; //Iteratory
	if(argc != 3)
	{
		perror("Bledna liczba argumentow. \n");
		exit(1);
	}
	
	wyraz = argv[1];
	katalog = argv[2];
	
	
	//wyraz = "Ala";
	//katalog = "tempFolder";
	
	size_t wyrazLen = strlen(wyraz);
		
	printf("Dlugosc wyrazu: %d\n", wyrazLen);
	
	
	if ((dir = opendir(katalog)) == NULL)
	{
		perror("Blad opendir");
		exit(1);
	}
	printf("Otwarto katalog\n");
	
	errno = 0;
	while ((pozycja = readdir(dir)) != NULL)
	{
		fileCounter = 0;
		filePath = pozycja->d_name;
		if(strcmp(filePath, "..\0") == 0 || strcmp(filePath, ".\0") == 0)
			continue;
			
		printf("Szukanie w pliku %s\n", filePath);
		
		if ( (fd = open(filePath, O_RDWR)) == -1)
		{
			printf("Blad otwarcia pliku %s\n", filePath);
			exit(1);
		}
		
		if( fstat(fd, &fileStat) )
		{
			perror("Blad odczytu rozmiaru pliku.\n");
			exit(1);
		}
		
		fileSize = (int)fileStat.st_size;
		printf("Rozmiar pliku: %d\n", fileSize);
		
		if( (adres = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (void*)-1 )
		{
			perror("Blad mmap.\n");
			exit(1);
		}
		close(fd);
		
		printf("Zmapowano\n");
		char temp[wyrazLen]; 
		
		
		for(i=0; i<fileSize; i++)
		{
			memcpy(temp, &adres[i], wyrazLen);
			temp[wyrazLen] = '\0';
			printf("temp: %s\n", temp);
			
					
			if(strcmp(wyraz, temp) == 0)
			{
				printf("Znaleziono w: %s\n", filePath);
				fileCounter ++;
			}

		}
		printf("W %s znaleziono wyraz %d razy \n", filePath, fileCounter);
						
	}
		
	if (errno)
	{
		perror("Blad readdir");
		exit(1);	
	}
	
	if (closedir(dir) == -1)
	{
		perror("Blad closedir");
		exit(1);
	}
	
	return 0;
}
