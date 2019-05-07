#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

// WYWOLANIE: ./wspak "Ala" "pom.txt"   
void reverseString(char*);
int main( int argc, char** argv)
{
	char *wyraz, *filePath;
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
	filePath = argv[2];
	
	//wyraz = "Ala";
	//filePath = "pom.txt";
	
	size_t wyrazLen = strlen(wyraz);
	printf("Dlugosc wyrazu: %d\n", wyrazLen);
	
	if ( (fd = open(filePath, O_RDWR)) == -1)
	{
		perror("Blad otwarcia pliku \n");
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
		
		//fflush(stdout);
		memcpy(temp, &adres[i], wyrazLen);
		temp[wyrazLen] = '\0';
		//printf("temp: %s\n", temp);
		
				
		if(strcmp(wyraz, temp) == 0)
		{
			printf("Znaleziono\n");
			reverseString(temp);
			printf("Odwrocono!\n");
			write(fd, adres, wyrazLen);
		}

	}
	munmap(adres, fileSize);
	
	return 0;
}

void reverseString(char *str)
{
    /* skip null */
    if (str == 0)
    {
        return;
    }

    /* skip empty string */
    if (*str == 0)
    {
        return;
    }

    /* get range */
    char *start = str;
    char *end = start + strlen(str) - 1; /* -1 for \0 */
    char temp;

    /* reverse */
    while (end > start)
    {
        /* swap */
        temp = *start;
        *start = *end;
        *end = temp;

        /* move */
        ++start;
        --end;
    }
}


