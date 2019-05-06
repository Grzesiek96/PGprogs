/* fswap_mmap.c - the program swaps all bytes of the file:
 * the 1-st becomes the last and the last becomes the first,
 * the 2-nd is swapped with the last by one, etc.
 * Example of mmap() use.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
int main(int argc, char *argv[])
{

	char *buf;
	int fd = 0, n;
	off_t fsize;
	struct stat st;
	char *znak1, *znak2;	
	
	if (argc == 4) {
		fd = open(argv[1], O_RDWR);//otwarcie pliku do czytania i pisania
		if (fd < 0) {
			fprintf(stderr,"Cannot open: %s\n", argv[1]);
			return 1;
		}
		znak1 = argv[2];
		znak2 = argv[3];
	
		printf("znak 1: %s znak2: %s\n", znak1, znak2);
		
	if (fstat(fd, &st)) {//odczytanie zawartości i-wezla pliku o deskryptorze zapisanym w fd
			perror("fstat(inputfile)\n");
			return 2;
		}
		fsize = st.st_size;//okreslenie rozmiaru pliku
		fprintf(stderr,"fsize=%lu\n", (unsigned long)fsize);
	} else {
		fprintf(stderr,"Usage: %s file\n", argv[0]);
		return 0;
	}
	fprintf(stderr,"pagesize() returns %lu\n", (unsigned long)getpagesize());
	buf = (char *)mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED,//zwraca wskaźnik na 1 element w i - weźle
			   fd, 0);
				//mmap w przypadku sukcesu zwraca adres mapowanego obszaru
			   
	if (buf == MAP_FAILED) {//MAP_failed that is (void *) -1 - rzutowanie NULL na wskaźnik
		perror("buf=mmap():");
		return 3;
	}
	fprintf(stderr,"swapping %lu bytes\n", (unsigned long)fsize);
	
	for (n = 0; n < fsize -1; n++) {
		if(znak1[0] == buf[n])
			buf[n]= znak2[0];
	}

	if (msync(buf, fsize, MS_SYNC) < 0)//zapewnia że zmiany zostana zapisane na dysku przed usunieciem odwzorowania
		perror("msync");
	munmap(buf, fsize);//usniecie odwzorowania pamieci
	close(fd);
	exit(0);
}
