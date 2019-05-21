/* program tworz¹cy nowe ³¹cze nazwane */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv, char **envp){
 mode_t mode = 0666;

 if(argc!=2){
  printf("Uzycie: producent <name>\n");
  exit(EXIT_FAILURE);
 }

 if(mkfifo(argv[1],mode)<0){
  perror("Blad: mkfifo()\n");
  exit(EXIT_FAILURE);
 }

 return 0;
}
