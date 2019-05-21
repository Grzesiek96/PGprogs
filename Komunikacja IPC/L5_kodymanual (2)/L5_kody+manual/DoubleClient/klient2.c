#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>

#define MSGSIZE 80

char *fifo2we = "fifo2we";
char *fifo2wy = "fifo2wy";

main(int argc, char **argv)
{
  int fd3, fd4;
  char bufor[MSGSIZE+1], ch;
  int wczytano;
  fd_set set, master;

  if ((fd3 = open(fifo2we, O_RDONLY | O_NONBLOCK))< 0)
  {
    perror("Blad open1");
    exit(1);
  };
  if ((fd4 = open(fifo2wy, O_WRONLY | O_NONBLOCK))< 0)
  {
    perror("Blad open2");
    exit(1);
  };

  FD_ZERO(&master);
  FD_SET(1, &master);
  FD_SET(fd3, &master);

  while(set = master, select(4, &set, NULL, NULL, NULL) > 0)
  {
    if(FD_ISSET(1, &set))
    {
      read(1, &ch, 1);       // lub read(0,&ch,1)
      write(fd4, &ch, 1);    //lub write(1,bufor,wczytano)
     };
    if(FD_ISSET(fd3, &set))
    {
      if((wczytano = read(fd3, bufor, MSGSIZE))>0)
      {
        write(0, bufor, wczytano);
      }
    };
  }
}
