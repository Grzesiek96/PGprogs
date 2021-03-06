/* mrecv1.c - prosty program do odczytu wiadomosci z kolejki komunikatow
 * Autor: M. Borkowski.
 */
#define _GNU_SOURCE 
#include <unistd.h>
#ifndef LINUX
#define TEMP_FAILURE_RETRY(expression) \
  (({ long int __result;                                                     \
       do __result = (long int) (expression);                                 \
       while (__result == -1L && errno == EINTR);                             \
       __result; }))
#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "msendrcv.h"

int main(int argc, char** argv) {
	int queue;
	packet p1;
	if((queue = msgget(QUEKEY,IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP))<0){
		perror("Create queue:");
		exit(EXIT_FAILURE);
	}
			
	for(;;){
		if(TEMP_FAILURE_RETRY(msgrcv(queue,&p1,PACKET,1,0))<0) break; //pobiera z kolejki jeden komunikat wskazanego typu
		printf("%s",p1.mtext);
	}
	if(errno) perror("mrecv1 error");
	return EXIT_SUCCESS;
}
