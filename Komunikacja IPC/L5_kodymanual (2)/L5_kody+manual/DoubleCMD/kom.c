#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sys/resource.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#define pro1 "./wys"
#define pro2 "zsh"

int main(int argc, char* argv[]){

int sel, n;
pid_t pid1, pid2;
fd_set wset, rset;
int f1in[2], f2in[2], f1out[2], f2out[2], maxfd;
struct timeval tv;
char buf[1024];

FD_ZERO(&wset);
FD_ZERO(&rset);

if(pipe(f1in)||pipe(f2in)||pipe(f1out)||pipe(f2out)){
    perror("BLAD1");
    exit(-1);
}

FD_SET(f2in[0], &rset);
FD_SET(f1in[0], &rset);


switch(pid1=fork()){
    case -1:
	perror("BLAD7");
	exit(-1);
    case 0:
	if(dup2(f1in[1],1)==-1||dup2(f1out[0],0)==-1){
	    perror("BLAD8");
	    exit(-1);
	}
	
	
	
        close(f2out[0]);
        close(f2in[0]);
        //close(f1out[0]);	
	close(f1in[0]);
	close(f2out[1]);
	//close(f2in[1]);
	close(f1out[1]);
	close(f1in[1]);
	
	execlp(pro1, pro1, NULL);
	perror("BLAD3");
	exit(-1);
}

switch(pid2=fork()){
    case -1:
	perror("BLAD9");
	exit(-1);
    case 0:
	if(dup2(f2out[0],0)==-1||dup2(f2in[1],1)==-1){
	    perror("BLAD10");
	    exit(-1);
	}
	
	
	//close(f2out[0]);
        close(f2in[0]);
        close(f1out[0]);	
	close(f1in[0]);
	close(f2out[1]);
	//close(f2in[1]);
	close(f1out[1]);
	close(f1in[1]);
	
	
	execlp(pro2, pro2, NULL);
	perror("BLAD5");
	exit(-1);
}

close(f2out[0]);
close(f2in[1]);
close(f1out[0]);
close(f1in[1]);

tv.tv_sec=5;

maxfd=(f1in[0]<f2in[0])?f2in[0]:f1in[0];

while((sel=select(maxfd+1, &rset, NULL, NULL, NULL))>=0){
    if(FD_ISSET(f1in[0], &rset)){
	n=read(f1in[0], buf, 1024);
	write(f2out[1], buf, strlen(buf)+1);
	printf("z deskryptora: %d otrzymalem: %s %d Wysylam dalej\n", f1in[0], buf, n);
    }
memset(buf, 0, 1024);
    if(FD_ISSET(f2in[0], &rset)){
	n=read(f2in[0], buf, 1024);
	write(f1out[1], buf, strlen(buf)+1);
	printf("z deskryptora: %d otrzymalem: %s %d Wysylam dalej\n", f2in[0], buf, n);
    }


memset(buf, 0, 1024);

FD_ZERO(&rset);
FD_SET(f1in[0], &rset);
FD_SET(f2in[0], &rset);    

}

return 0;
}


