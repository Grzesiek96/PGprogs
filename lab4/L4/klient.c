#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#define FIFO_FILE1 "MYFIFO1"

int main(int argc, char *argv[])
{
 // FILE *fp1, *fp2;
	int fd1, fd2, maxfd, ret, towrite;
	fd_set readfds;
	char buf[80]

  if ( argc != 2 ) {
    printf("Wywolanie: fifoclient [napis]\n");
    exit(1);
  }
	fd1 = 0;
	fd2 = open(FIFO_FILE1, O_RDONLY);
	for (;;) {
		maxfd = (fd1 > fd2) ? fd1 : fd2; // jesli warunek jest prawdziwy przypisz wartość fd1 a jeśli nie to fd2
		if (maxfd < 0)
			break;//select zwroci -1 po odbiorze sygnału
		FD_ZERO(&readfds);//ustawiamy maskę do odczytu
		if (fd1 >= 0)
			FD_SET(fd1, &readfds);//ustawienie maski danego deskryptora
		if (fd2 >= 0)
			FD_SET(fd2, &readfds);//ustawienie maski fd2
		if (select(maxfd + 1, &readfds, 0, 0, 0) < 0) {
			if (errno == EINTR)
				continue;
			else {
				perror("select");
				return 3;
			}
		}
	}
	if (fd1 >= 0 && FD_ISSET(fd1, &readfds)) {
			if ((towrite = read(fd1, buf, sizeof(buf))) < 0) {//sprawdzenie ile bajtow sie zapisalo
				perror("read 1");
				return 4;
	}
			if (towrite > 0) {
				p = buf;
				while (towrite > 0) {
					ret = write(1, p, towrite);//na port wyjscia, p-wskaznik, towrite- liczba bajtow do zapisania
			//liczba bajtow do zapisania nie zawsze musi zapisać sie na raz
					if (ret < 0) {
						perror("write 1");
						return 5;
					}
					towrite -= ret;
					//dlatego zmniejszamy wartość towrite o liczbe zapisanych bajtow
					//wartość wskaznika p zostaje zwięszkona o przesuniecie ret(już wczytane bajty)
					p += ret;
				}
			} else {// w przeciwnym wypadku read uzyskał zero (koniec źródła danych
				close(fd1);
				fd1 = -1;//nigdy nie bedziemy ustawiać ani sprawdzać wartości deskryptora
				fprintf(stderr, "\n\nEnd of data 1\n\n");
			}
		}
		if (fd2 >= 0 && FD_ISSET(fd2, &readfds)) {//deskryptor, który może być  związany z łączem albo z plikiem
			if ((towrite = read(fd2, buf, sizeof(buf))) < 0) {
				perror("read 2");
				return 4;
			}
			if (towrite > 0) {
				usleep(1000000);
				p = buf;
				while (towrite > 0) {
					ret = write(2, p, towrite);
					if (ret < 0) {
						perror("write 2");
						return 5;
					}
					towrite -= ret;
					p += ret;
				}
			} else {
				close(fd2);
				fd2 = -1;
				fprintf(stderr, "\n\nEnd of data 2\n\n");
			}

		}
		sleep(1);
	}
	return 0;
}

 /* if((fp1 = fopen(FIFO_FILE1, "w")) == NULL) {
    perror("fopen");
    exit(1);
  }
   if((fp2 = fopen(FIFO_FILE1, "r")) == NULL) {
    perror("fopen");
    exit(1);
  }*/

  //fputs(argv[1], fp1);

//  fclose(fp1);
 // return(0);
//} 
