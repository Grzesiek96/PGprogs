/* Napisaæ program potok wywo³ywany nastêpuj¹co:
potok cmd1 cmd2 wynik
Program ten tworzy dwa procesy potomne za pomoc¹ poleceñ cmd1 oraz cmd, przekierowuj¹c standardowe wyjœcie pierwszego procesu na standardowe wejœcie drugiego, oraz standardowe wyjœcie drugiego procesu do pliku (tu: wynik). Przyk³adowo, wywo³anie:
potok ls less lista
powinno dzia³aæ tak, jak polecenie pow³oki (np. zsh):
ls | less > lista */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


int main(int argc,char *argv[]){
   
   char buf[80],c;
   int fd1,fd2,fd[2];
   char *cmd1,*cmd2,*wynik;
   pid_t pid;
   
   
   while((c=getopt(argc,argv,""))!=-1){
      switch(c){
         case '?':
            printf("Nieznany argument -%c\n",optopt);
            break;
      }
   }
   
   if (optind+2<argc){
      cmd1=argv[optind];
      cmd2=argv[optind+1];
      wynik=argv[optind+2];
   }
   printf("%s | %s > %s\n",cmd1,cmd2,wynik);
   
   if((fd1=open(wynik,O_CREAT|O_TRUNC|O_RDWR))==-1){
      perror("open");
      exit(1);
   }
   
   if(pipe(fd)==-1){
      perror("pipe");
      exit(1);
   }
   
   if ((pid=fork())==0){
      printf("%d %s\n",getpid(),cmd2);
      close(0);
      close(1);
      dup2(fd[0],0);
      dup2(fd1,1);
      
      execlp(cmd2,NULL);
      perror("exec");
   }   
   else if(pid>0){
      printf("%d %s\n",getpid(),cmd1);
      close(1);
      dup2(fd[1],1);
      execlp(cmd1,NULL);
      perror("exec");
   }
   else{
      perror("fork");
      exit(1);
   }
   
   close(fd1);
   return 0;
}
