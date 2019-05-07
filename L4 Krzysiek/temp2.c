#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char *wyraz = "Ala";
	char * filePath = "pom.txt";
	int fd;
	//wyraz = "Ala";
	//filePath = "pom.txt";
	int fileSize;
	struct stat fileStat;
	
	if ( (fd = open(filePath, O_RDWR)) == -1)
	{
		perror("Blad otwarcia pliku \n");
		exit(1);
	}
	printf("Otawrto plik\n");
	if( (fstat(fd, &fileStat)) )
	{
		perror("Blad odczytu rozmiaru pliku.\n");
		exit(1);
	}
	
	fileSize = (int)fileStat.st_size;
	printf("Rozmiar pliku: %d\n", fileSize);

	return 0;
}
