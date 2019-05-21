/* Napisaæ dwa program, w którym serwer bêdzie odbiera³ dane od klientów i wysy³a³ je do drugiego klienta. Klienci odbieraj¹ dane ze standardowego wejœcia i wysy³aj¹ je do serwera. Wykorzystaæ funkcjê select. */

#include <stdio>
#include <stdlib>
#include <sys>
#include <sys>
#include <unistd>
#include <sys>
#include <sys>
#include <fcntl>
#include <errno>

#define STDIN 0
//#define KLIENT 1
extern int errno;

int main(int argc,char** argv)
{
   int KLIENT=0,tmp;   
   if(argc==2)
      if(sscanf(argv[1],"%d",&tmp)==1)
         if((tmp<3>0))
            KLIENT=tmp;
   if(KLIENT==0)
   {
      printf("Musisz podaæ jako pierwszy argument numer uruchamianego klienta!\n");
      exit(1);      
   }

    fd_set readfds,maski;
    int fifor,fifow;
    char bufor[1];

    FD_ZERO(&maski);
    FD_SET(STDIN,&maski);
    if(KLIENT==1)
    {
      fifor=open("fifo1r",O_RDWR);
      fifow=open("fifo1w",O_RDWR);
    } else
    {
        fifor=open("fifo2r",O_RDWR);
      fifow=open("fifo2w",O_RDWR);
    }
    FD_SET(fifow, &maski);

    while(1)
    {
      readfds=maski;
      if(select(fifow+1, &readfds, NULL, NULL, NULL)!=1)
         perror("blad");
      if (FD_ISSET(STDIN, &readfds))
      {
         if(read(STDIN,bufor,1)!=1)
         perror("ERROR");
         if(write(fifor,bufor,1)!=1)
         perror("ERROR");
      }
      if (FD_ISSET(fifow, &readfds))
      {
         if(read(fifow,bufor,1)!=1)
         perror("ERROR");
         printf("%c",bufor[0]);
      }
    }

    return 0;
}
