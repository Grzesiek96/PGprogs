#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>

#define FIFO_FILE1 "MYFIFO1"
//#define FIFO_FILE2 "MYFIFO2"



int main(void)
{
  FILE *fp1, *fp2, *fp3, *fp4;
  char readbuf[80];

/*tworzenie FIFO*/
  umask(0);
  mknod(FIFO_FILE1, S_IFIFO|0666, 0);//uruchamiamy łącza nazwane

  while(1)
  {
    fp1 = fopen(FIFO_FILE1, "r");
    fgets(readbuf, 80, fp1);//potwierdzenie zakonczenia czytania
   // n = read(fp1, readbuf, sizeof(buf));
    printf("Orzymany tekst: %s\n", readbuf);
    fp2 = fopen(FIFO_FILE1, "w");
    int m,n;
   
	n = read(fp1, readbuf, sizeof(readbuf));
		if (n > 0) {
			if ((m = write(fp2, readbuf, n)) != n) {
				fprintf(stderr, "m=%d, n=%d\n", m, n);
				return 2;
			}
		}
    printf("WYSLANY tekst: %s\n", readbuf);//potwierdzenie zakonczenia zapisywania
    fclose(fp1);
    fclose(fp2);
}
  
//serwer ma także usuwać NODE
  return(0);
}
//klient czyta ze standardowego wejścia, to co odczyta kopiuje do łącza

//to co serwer odbierze złącza ma wysłać do procesu

//jak serwer odczyta coś z jednego łącza to tym łączem wysyła do drugiego klienta

//serwer obsluguje 4 łącza
