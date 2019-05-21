#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>

int main(int argc, char **argv, char **envp){
 int fd;
 int wczytano;
 char bufor[PIPE_BUF];

 if(argc!=2){
  printf("Uzycie: konsument <name>\n");
  exit(EXIT_FAILURE);
 }


 if((fd = open(argv[1],O_RDONLY))<0){
  perror("Blad: open()\n");
  exit(EXIT_FAILURE);
 }

 while( (wczytano = read(fd,bufor,PIPE_BUF-1)) > 0 ){
   printf("konsument przeczytal:\t%s\n",bufor);
 }

 close(fd);
 exit(EXIT_SUCCESS);
}
