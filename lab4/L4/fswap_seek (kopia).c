/* fswap_seek.c - the program swaps all bytes of the file:
 * the 1-st becomes the last and the last becomes the first,
 * the 2-nd is swapped with the last by one, etc.
 * Example of lseek() use.
 */
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
int main(int argc, char *argv[])
{
	int fd;
	off_t fsize, n, n2;
	struct stat st;
	if (argc == 2) {
		fd = open(argv[1], O_RDWR);
		if (fd < 0) {
			perror("open");
			return 1;
		}
		if (fstat(fd, &st)) {//odczytanie zawartości i- wezla wczytanego pliku wg deskryptora uzyskanego przez open
			perror("fstat(inputfile)\n");
			return 11;
		}
		fsize = st.st_size;
		fprintf(stderr, "fsize=%lu\n", (unsigned long)fsize);
	} else {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		return 0;
	}
	n2 = fsize / 2;
	for (n = 0; n < n2; n++) {
		char c1, c2;
		//================lseek ustawia wskaźnik na dowolną pozycje
		if (lseek(fd, n, SEEK_SET) == (off_t) - 1) {//SEEK_SET - offset pliku jest ustawiony na liczbe bajtow n
			perror("lseek 1");
			exit(1);
		}
		if (read(fd, &c1, 1) <= 0) {//wczytuje z deskryptora fd do adresu c1 (1-ilosc bajtow)
			perror("read 1");
			exit(2);
		}
		if (lseek(fd, fsize - n - 1, SEEK_SET) == (off_t) - 1) {// ustawienie wskaźnika na ostatnim elemencie (odejmujemy jeszcze 1 bo numeracja od 0)
			perror("lseek 2");
			exit(3);
		}
		if (read(fd, &c2, 1) <= 0) {//wczytanie ostatniego bajtu
			perror("read 2");
			exit(4);
		}
		if (lseek(fd, fsize - n - 1, SEEK_SET) == (off_t) - 1) {//należy powrocic na ostatni element, gdyż wskaźnik został przesunięty przez read
			perror("lseek 3");
			exit(5);
		}
		if (write(fd, &c1, 1) <= 0) {
			perror("write 1");
			exit(4);
		}
		if (lseek(fd, n, SEEK_SET) == (off_t) - 1) {
			perror("lseek 4");
			exit(1);
		}
		if (write(fd, &c2, 1) <= 0) {
			perror("write 2");
			exit(4);
		}
	}
	printf("\nFinished\n");
	close(fd);
	exit(0);
}
