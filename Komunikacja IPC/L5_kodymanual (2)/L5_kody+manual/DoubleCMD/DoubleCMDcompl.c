/* Napisaæ program potok wywo³ywany nastêpuj¹co:
potok cmd1 cmd2 wynik
Program ten tworzy dwa procesy potomne za pomoc¹ poleceñ cmd1 oraz cmd, przekierowuj¹c standardowe wyjœcie pierwszego procesu na standardowe wejœcie drugiego, oraz standardowe wyjœcie drugiego procesu do pliku (tu: wynik). Przyk³adowo, wywo³anie:
potok ls less lista
powinno dzia³aæ tak, jak polecenie pow³oki (np. zsh):
ls | less > lista */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
/*#include <wait.h>*/
#include <sys/wait.h>
#include <sys/resource.h>
#include <fcntl.h>

#define INPUTSIZ        256
#define MAXPAR   20

void doshell(char *cmd) {
   char *argv[MAXPAR+1], *p;
   int argc=0, status;
   int fd1[2], fd;
   int mask;
        pid_t pid;
   struct rusage ru;
/* Dzielenie linii polecen na "wyrazy" */
   if((argv[argc]=strtok(cmd," \t\n"))==NULL) return;
   while(argv[argc] && argc<MAXPAR-1){
      argv[++argc]=strtok(NULL," \t\n");
   }
   
   // tworzenie pliku wynikowego
   
   umask(0); // zeby maska nie modyfikowa³a praw tworzonego pliku
   sscanf("0600", "%o", &mask);
   fd = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, mask);
   
   if(pipe(fd1) == -1)
   {
       perror("pipe");
       exit(1);
   }
   
        if ( (pid = fork()) < 0) {
      perror("Blad wywolania fork"); exit(2);
        }
        if (pid == 0) {
      dup2(fd1[1], 1);
      close(fd1[1]);
      close(fd1[0]);
                execlp(argv[0],argv[0], NULL);
                perror("Blad wywolania execlp"); exit(1);
        }
   else
   {
      dup2(fd1[0], 0);
      dup2(fd, 1);
      close(fd1[1]);
      close(fd1[0]);
           execlp(argv[1],argv[1], NULL);
                perror("Blad wywolania execlp"); exit(1);
   }
}
int main(int argc, char* argv[]) {
        char buf[INPUTSIZ];
   int i, x;
   memset(&buf, '\0', INPUTSIZ);
   for(i=1;i<argc-1;i++)
   {
     for(x=0;argv[i][x] != '\0';x++)
     {}
     argv[i][x]=' ';
   }
   strcat(buf, argv[1]);
   //printf("\n%s\n", buf);
   doshell(buf);

        /*printf("\n> ");
        while (fgets(buf, INPUTSIZ, stdin) != NULL) {
           //printf("\n%s\n", buf);
                doshell(buf);
                printf("\n> ");
        }
        exit(0);*/
}
