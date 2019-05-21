#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

int main(){
  char buf[BUFSIZ];
  int fp;
 
  if(mkfifo("fifo",0655)==-1 && errno!=EEXIST){
    //dzieki errno!=EEXIST jesli plik fifo istnieje to nam sie program nie wywali
    perror("mkfifo");
    exit(1);
  }
 
  if((fp=open("fifo",O_RDWR))==-1){
    perror("open fifo");
    exit(1);
  }
 
  for(;;){
    read(fp,buf,BUFSIZ);
    printf("MSG: %s",buf);
  }
}
