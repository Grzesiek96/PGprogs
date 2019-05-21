#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#define MSGSIZE 80

char *fifo1we = "fifo1we";
char *fifo1wy = "fifo1wy";
char *fifo2we = "fifo2we";
char *fifo2wy = "fifo2wy";


main(int argc, char **argv)
{
  int fd1, fd2, fd3, fd4;
  char bufor[MSGSIZE+1];
  int wczytano;
  fd_set set, master;
   
  if (mkfifo(fifo1we, 0666)== -1)
  {
    if(errno!=EEXIST)
    {
      perror("Blad mkfifo1");
      exit(1);
    };
  };
  if (mkfifo(fifo1wy, 0666)== -1)
  {
    if(errno!=EEXIST)
    {
      perror("Blad mkfifo1");
      exit(1);
    };
  };
 
  if (mkfifo(fifo2we, 0666)== -1)
  {
    if(errno!=EEXIST)
    {
      perror("Blad mkfifo2");
      exit(1);
    };
  };
 
  if (mkfifo(fifo2wy, 0666)== -1)
  {
    if(errno!=EEXIST)
    {
      perror("Blad mkfifo2");
      exit(1);
    };
  };
   if ((fd1 = open(fifo1we, O_RDWR))< 0)
   {
     perror("Blad open1");
     exit(1);
   };
   
   if ((fd2 = open(fifo1wy, O_RDWR ))< 0)
   {
     perror("Blad open2");
     exit(1);
   };

   if ((fd3 = open(fifo2we, O_RDWR ))< 0)
   {
     perror("Blad open3");
     exit(1);
   };

   if ((fd4 = open(fifo2wy, O_RDWR))< 0)
   {
     perror("Blad open4");
     exit(1);
   };


  FD_ZERO(&master);
  FD_SET(fd4, &master);
  FD_SET(fd2, &master);

  while(set = master, select(fd4+1, &set, NULL, NULL, NULL) > 0)
  {
    if(FD_ISSET(fd2, &set))
    {
      write(0, "Klient1 -> Klient2\n", 20);
      if((wczytano = read(fd2, bufor, MSGSIZE))>0)
      {
        write(fd3, bufor, wczytano);
      }
    };
    if(FD_ISSET(fd4, &set))
    {
      write(0, "Klient2 -> Klient1\n", 20);
      if((wczytano = read(fd4, bufor, MSGSIZE))>0)
      {
   write(fd1, bufor, wczytano);
      }
    };
  } 
}
