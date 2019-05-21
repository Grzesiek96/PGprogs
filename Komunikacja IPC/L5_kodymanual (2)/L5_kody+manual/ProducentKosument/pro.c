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

int nid;

void alrm(int sn){
    close(nid);
    unlink("dupa");
    exit(0);
}


int main(int argc, char* argv[]){

int czs;
char buf[1024];

if(argv[1]==NULL)
    czs=0;
else
    czs=atoi(argv[1]);
    
signal(SIGALRM, alrm);
    
if(alarm(czs)<0){
    perror("BLAD");
    exit(-1);
}

if((nid=mknod("dupa", S_IFIFO|0700, NULL))<0)
    if(errno!=EEXIST){
	perror("BLAD");
	exit(-1);
    }

if((nid=open("dupa", O_WRONLY, NULL))<0){
    perror("BLAD");
    exit(-1);
}

while(1){
    read(0,buf, 1024);
    write(nid, buf, strlen(buf));
}


return 0;
}


