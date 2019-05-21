/* Proces macierzysty uruchamia P procesów potomnych, które bêd¹ wykonywaæ identyczne obliczenia (suma N_i kolejnych liczb naturalnych). Nastêpnie odczytuje z terminala (stdin) kolejno P liczb naturalnych N_1 ... N_p i przesy³a po jednej liczbie ka¿demu z procesów potomnych. Procesy startuj¹ obliczenia po otrzymaniu liczby N_i od rodzica. Po zakoñczeniu obliczeñ ka¿dy proces potomny musi przes³aæ wyniki do procesu macierzystego. Proces macierzysty wypisuj¹ na stdout PID procesu potomnego oraz odebrany wynik obliczeñ. Do komunikacji wykorzystaæ ³¹cza nienazwane.
(punktacja : 1p za koncepcjê + 2 p za komunikacjê + 2p za select)

Modyfikacja od prowadz¹cych : dane mo¿n¹ trzeba przesy³aæ tylko w jedna stronê za pomoc¹ ³¹czy, a w druga dowolnym sposobem. Ale sensowne zastosowanie funkcji select wymusza³o zastosowanie ³¹czy od potomnych do macierzystego. */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>


int main(int argc, char *argv[])
{
    int i=0,j=0,k=0,P=0,*n,**fd;
    int suma=0;
    fd_set rid;
    pid_t pid,*pidy,pidi=1;   
    if( argc != 2 )
    {
   printf("Zly format \n");
   exit(1);
    }
    errno=0;
    P=strtol(argv[1],(char **) NULL, 10);
    if(errno)
    {
   printf("Zly format\n");
   exit(1);
    }
    if((n=malloc ( P * sizeof (int) ))==NULL)
    {
   printf("Blad malloc \n");
   exit(1);
    }
    fflush(stdin);   
    for( i = 0 ; i < P ; i++)
    {
   printf("Podaj liczbe N%d \n",i+1);
   errno=0;
   scanf("%d",n+i);
   fflush(stdin);
   if(errno || n[i] < 1)
   {
       printf("Blad \n");
       i--;
   }   
    }   
    if((fd=malloc(sizeof(int *) * P))==NULL)
    {
   free(n);
   printf("Blad malloc \n");
   exit(1);
    }   
    if((pidy=malloc(sizeof(pid_t) * P))==NULL)
    {
   free(n);
   free(fd);
   printf("Blad malloc \n");
   exit(1);
    }   
    for( i= 0 ; i < P && pidi!=0 ; i++)
    {
   
   if((fd[i]=malloc(2 * sizeof(int)))==NULL)
   {
       free(n);
       free(fd);
       free(pidy);
       printf("Blad malloc \n");
       exit(1);
   }
   if(pipe(fd[i]) < 0)
   {    printf("Blad pipe");
        exit(1);
   }   
   if((pidi=pidy[i]=fork())< 0)   
   {
       printf("Blad fork \n");
       exit(1);
   }
   if(pidi == 0) break;
    }
    if( i== P && pidy[P-1] > 0)       //w macierzystym
    {   
   for( i= 0 ; i < P  ; i++)
       close(fd[i][1]);
      
   for( k= 0 ; k < P  ; k++)
   {
      //printf("k= %d \n",k);
      FD_ZERO(&rid);
      FD_SET(fd[k][0],&rid);
      errno=0;
      if((select(fd[k][0] + 1,&rid,NULL,NULL,NULL))==-1)
      {
         perror("Select");
              exit(1);
      }           
      if( FD_ISSET(fd[k][0],&rid) )          
          {   
         if( ( read( fd[k][0],&j,sizeof(int) ) )!=0 )
                  printf("Suma w macierzystym %d z potomka %d o pid %d \n",j,k+1,pidy[k]);
         close(fd[k][0]);
      }
      free(fd[k]);      
   }
   free(fd);
   free(pidy);
   free(n);      
    }
    else
    {
   suma=0;
   for( j = 0 ; j < i ; j ++)
   {   
      close(fd[j][0]); //zamykam deskryptory do komunikacji miedzy macierzystym a wszystkimi pozostalymi potomkami
      close(fd[j][1]);
   }
   close(fd[i][0]);
   for( j = 0 ; j < n[i] + 1 ; j ++)
       suma+=j;   
   if((write(fd[i][1],&suma,sizeof(int)))==0)
      printf("NIe Zapisano \n");
   close(fd[i][1);
   return 0;
    }
}
