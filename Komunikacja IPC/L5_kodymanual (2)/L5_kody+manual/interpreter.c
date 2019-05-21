#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
int main()
{
   char bufor[80];
   char* arg[10];
   int bg;

   while (1)
   {
      printf("msh $ ");
      fgets(bufor, 80, stdin);
      bg = AnalizujPolecenie(bufor, arg);
      
      if (arg[0] == NULL)
         continue;
      else if (strcmp(arg[0], "exit")==0)
         exit(0);
      else
      {
         Wykonaj(arg, bg);
      }
   }
}

int AnalizujPolecenie(char *bufor, char *arg[])
{
   int counter=0;
   int i=0, j=0;

   while (bufor[counter] != '\n')
   {
      while (bufor[counter] == ' ' || bufor[counter] == '\t')
                   counter++;
      if (bufor[counter] != '\n')
      {
         arg[i++] = &bufor[counter];
         while (bufor[counter] != ' ' && bufor[counter] != '\t' && bufor[counter] != '\n')
            counter++;
                   if (bufor[counter] != '\n')
                      bufor[counter++] = '\0';
      }
   }
   bufor[counter] = '\0';
   arg[i]=NULL;
   if (i>0)
      while (arg[i-1][j] != '\0')
      {
         if (arg[i-1][j] == '&')
         {
            if (j == 0)
               arg[i-1] = NULL;
            else
               arg[i-1][j] = '\0';
            return 1;
         }
         j++;
      }
   return 0;
}

void ZastapProces(char *arg[])
{
// Laboratorium 4 !!!
  int i=1;
  int fd;
  while(i<9 && arg[i]!=NULL)
  {
   if (!strcmp(arg[i], ">") || !strcmp(arg[i], "<") || !strcmp(arg[i],">>")) // jesli sa przekierowania
   {
      if (!strcmp(arg[i], ">") && arg[i+1]!=NULL) // do strumienia
      {         
         fd=open(arg[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
         if (fd==-1)
         {
            perror("Blad open");
            exit(1);
         }
         if (dup2(fd, 1)==-1)
         {
            perror("Blad przekierowania");
            exit(1);
         }
         arg[i]=NULL;
         break;
      }
      
      if (!strcmp(arg[i], ">>") && arg[i+1]!=NULL) // do strumienia
      {         
         fd=open(arg[i+1], O_WRONLY | O_APPEND , 0666);
         if (fd==-1)
         {
            perror("Blad open");
            exit(1);
         }
         if (dup2(fd, 1)==-1)
         {
            perror("Blad przekierowania");
            exit(1);
         }
         arg[i]=NULL;
         break;
      }
         
      else if (!strcmp(arg[i], "<") && arg[i+1]!=NULL) // ze strumienia
      {
         int fd;
         fd=open(arg[i+1], O_RDONLY);
         if (fd==-1)
         {
            perror("Blad open");
            exit(1);
         }
         if (dup2(fd, 0)==-1)
         {
            perror("Blad przekierowania");
            exit(1);
         }
         arg[i]=NULL;
         break;
      }
      }
      else if (!strcmp(arg[i], "|")) // potok
      {
         arg[i]=NULL;
         int des[2];
         if (pipe(des))
         {
            perror("Blad utworzenia potoku");
            exit(1);
         }
         arg[i]=NULL;
         int pid=fork();
         if (pid==-1)
         {
            perror("Blad fork dla laczenia we/wy");
            exit(1);
         }
         if (pid==0)
         {
            close(des[0]);
            if(dup2(des[1], 1)==-1)
            {
               perror("Blad dup2 pot");
               exit(1);
            }
            Wykonaj(arg);
         }
         else
         {
            close(des[1]);
            if(dup2(des[0], 0)==-1)
            {
               perror("Blad dup2 mac");
               exit(1);
            }
            Wykonaj( arg+i+1);
         }
      }
   i++;
  }
  execvp(arg[0], arg);
}

int Wykonaj(char *arg[] , int bg)
{
  pid_t pid;
  int status;
  int i=0;

  pid=fork();
  if (pid==-1)
  {
    perror("fork");
    exit(1);
  }
  if (pid==0)
  {
   ZastapProces(arg); // Wykonanie polecenia - lab. 4!!!   <   >   |
   perror("execvp");
   exit(1);
  }
  else if (bg==0)
  {
    int status;
    if (waitpid(pid, &status, 0)!=pid)
      printf("Blad: proces potomny nie zostal stworzony\n");
  }
}
