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
  printf("Uzycie: producent <name>\n");
  exit(EXIT_FAILURE);
 }

 if((fd = open(argv[1],O_WRONLY))<0){
  perror("Blad: open()\n");
  exit(EXIT_FAILURE);
 }

 while(1){
   printf("producent wyslal\n");
   if(write(fd,"Hello World",12) <0){
    perror("blad: write()\n");
    exit(EXIT_FAILURE);
   }
   sleep(3);
 }

 close(fd);
 exit(EXIT_SUCCESS);
}
