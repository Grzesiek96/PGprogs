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

int main(int argc, char* argv[]){

int czs, nid;
char buf[1024];

if(argv[1]==NULL)
    czs=0;
else
    czs=atoi(argv[1]);
    
alarm(czs);

if((nid=open("dupa", O_RDONLY, NULL))<0){
    perror("BLAD");
    exit(-1);
}

while(1){
    if(read(nid, buf, 1024)<=0){
	perror("BLAD");
	exit(-1);
    }
write(1, buf, strlen(buf));
}





return 0;
}


