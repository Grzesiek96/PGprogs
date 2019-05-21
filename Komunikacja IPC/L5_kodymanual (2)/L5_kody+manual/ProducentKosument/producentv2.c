#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

int main(){
  char buf[BUFSIZ]; // BUFSIZ to makro z stdio.h zwracajace optymalny rozmiar bufora (na solarisach 1024) (wielkosc bloku dyskowego w systemie)
  int fp;
  int nread;

  if((fp=open("fifo",O_WRONLY))==-1){
    perror("open fifo");
    exit(1);
  }
  for(;;){
    if((nread=read(0,buf,BUFSIZ))==-1){
      perror("read");
      exit(1);
    }
    if(nread<BUFSIZ)
      buf[nread]='\0'; // ustawia koniec bufora zeby nam nie drukowalo krzakow
    write(fp,buf,BUFSIZ);
  }
}
