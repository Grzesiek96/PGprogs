/* Napisaæ dwa program, w którym serwer bêdzie odbiera³ dane od klientów i wysy³a³ je do drugiego klienta. Klienci odbieraj¹ dane ze standardowego wejœcia i wysy³aj¹ je do serwera. Wykorzystaæ funkcjê select. */

#include <stdio>
#include <sys>
#include <sys>
#include <unistd>
#include <sys>
#include <sys>
#include <fcntl>
#include <signal>
#include <stdlib>

   struct sigaction nowaAkcja;
   sigset_t maska;
/*
void error(void)
{
    perror("ERROR");
    exit(1);
}
*/
void zakoncz(int ns)
{
   unlink("fifo1r");
   unlink("fifo1w");
   unlink("fifo2r");
   unlink("fifo2w");
   printf("Koniec pracy programu\n");
   exit(1);
   return;
}

int main(void)
{
    fd_set readfds,maski;
    int fifo1r,fifo1w,fifo2r,fifo2w;
    char bufor[10];

   sigfillset(&maska);
   nowaAkcja.sa_handler = zakoncz;
   nowaAkcja.sa_mask = maska;
   if (sigaction(SIGINT, &nowaAkcja, NULL))       perror("Blad sigaction");


    if(mkfifo("fifo1r",0666)==-1)      perror("ERROR");
    if((fifo1r=open("fifo1r",O_RDWR))<0)   perror("ERROR");
    if(mkfifo("fifo1w",0666)==-1)      perror("ERROR");
    if((fifo1w=open("fifo1w",O_RDWR))<0)   perror("ERROR");
    if(mkfifo("fifo2r",0666)==-1)      perror("ERROR");
    if((fifo2r=open("fifo2r",O_RDWR))<0)   perror("ERROR");
    if(mkfifo("fifo2w",0666)==-1)      perror("ERROR");
    if((fifo2w=open("fifo2w",O_RDWR))<0)   perror("ERROR");

    FD_ZERO(&maski);
    FD_SET(fifo1r, &maski);
    FD_SET(fifo2r, &maski);

    while(1)
    {
   readfds=maski;
   if(select(fifo2w+1, &readfds, NULL, NULL, NULL)!=1) perror("ERROR");
   if (FD_ISSET(fifo1r, &readfds))
   {
       printf("1->2\n");
       if(read(fifo1r,bufor,1)!=1)   perror("ERROR");
       if(write(fifo2w,bufor,1)!=1) perror("ERROR");
   }
   if (FD_ISSET(fifo2r, &readfds))
   {
       printf("2->1\n");
       if(read(fifo2r,bufor,1)!=1)   perror("ERROR");
       if(write(fifo1w,bufor,1)!=1) perror("ERROR");
   }
    }

    return 0;
}
