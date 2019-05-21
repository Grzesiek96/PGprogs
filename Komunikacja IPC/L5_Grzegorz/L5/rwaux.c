/* sem.c - auxiliary functions to handle UNIX IPC shared memory segments
 * and semaphore sets.
 *  LJO. 2016-05-05. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include "rwaux.h"

int m_creat(char *name, int nr, int size){/* creates a new shm segment
				and initializes it all with zeros. If the segment already
				existed it gets its id */
	int shmid;
	key_t key;
	key=ftok(name, nr);
	if(((key_t)-1)==key){
		fprintf(stderr,"m_creat: bad name:<%s>\n",name); return -2;
	}
	shmid = shmget(key, size, IPC_CREAT|IPC_EXCL | 0600);//IPC_EXCL jest zabezpieczeniem jesli segment juz istnieje
	if(shmid<0){//zasób nie jest dostępny
		if(errno!=EEXIST){
			perror("m_creat"); return -3;
		}
		shmid=shmget(key, size, 0);
	} else {
		shmid=shmget(key, size, 0);
		if(shmid>0){
			char *p=shmat(shmid,(void *)0,0);
			if((char *)-1==p){
				perror("m_creat:shmat"); return -4;
			} else {
				memset(p,0,size);/* zeroing new segment */
			}
		}
	}
	return shmid;
}

void m_rm(char *name, int nr){/* marks shared memory segment for removal */
	int shmid;
	key_t key;
	key=ftok(name, nr);
	if(((key_t)-1)==key){
		fprintf(stderr,"m_rm: bad name:<%s>\n",name); return;
	}
	shmid = shmget(key, 1, 0);
	if(shmid<0){
		perror("m_rm:1"); return;
	}
	if(shmctl(shmid,IPC_RMID,0)<0){//oznacza segment do zniszczenia po odłączeniu ostatniego procesu
		perror("m_rm:2");
		return;
	}
}

int s_creat(char *name, int nr, int val) {
/* Creates a new single semaphore and initializes it to 'val'.
 * If the semaphore already existed it gets its id */
	int semid;//id procesu ostatnio wykonujacego operacje na sepaforze
	key_t key;//liczba identyfikująca zestaw semaforów
	key=ftok(name, nr);
	if((key_t)-1==key){
		fprintf(stderr,"s_creat: bad name:<%s>\n",name); return -2;
	}
	semid = semget(key, 1, IPC_CREAT|IPC_EXCL | 0600);
	if(semid<0){
		if(errno!=EEXIST){
			fprintf(stderr,"cannot create semaphore, ftok(%s,%d), key=%d\n",name,nr,key);
			perror("s_creat:1"); return -3;
		}
		semid = semget(key, 1, 0600);
		if(semid<0){
			fprintf(stderr,"cannot attach semaphore, ftok(%s,%d), key=%d\n",name,nr,key);
			perror("s_creat:2");
		}
	} else {
		union semun arg;
		arg.val=val;
		if(semctl(semid,0,SETVAL,arg)<0){//jesli drugim argumentem jest 0 to semctl ma wykonać oberację, w której liczba semaforów nie ma znaczenia
			perror("s_creat:SETVAL");   return -4;
		}
	}
	return semid;
}

int s_get(char *name, int nr) {
/* Gets an id of an existing semaphore */
	int semid;
	key_t key;
	key=ftok(name, nr);
	if((key_t)-1==key){
		fprintf(stderr,"s_get: bad name:<%s>\n",name); return -2;
	}	
	semid = semget(key, 1, 0);
	if(semid<0){
		fprintf(stderr,"cannot attach semaphore, ftok(%s,%d), key=%d\n",name,nr,key);
		perror("s_get");
	}
	return semid;
}

void s_wait(int semid, int flag) {/* wait operation */
	struct sembuf s;
	s.sem_num = 0;
	s.sem_op = -1;
	s.sem_flg = flag; /* 0 or SEM_UNDO */;
	if(semop(semid, &s, 1) < 0) { perror("s_wait"); exit(1);}
}

void s_post(int semid, int flag) {/* post operation */
	struct sembuf s;
	s.sem_num = 0;
	s.sem_op = 1;//
	s.sem_flg = flag;/* 0 or SEM_UNDO */;
	if(semop(semid, &s, 1) < 0) { perror("s_post"); exit(1);}
}

int s_getval(int semid){/* get actual semaphore value */
	int semval;
	semval = semctl(semid, 0, GETVAL, 0);//odwolanie do semafora
    if (semval==-1){ perror("s_getval"); exit(1);}
	return semval;
}

void s_rm(char *name, int nr){/* remove semaphore */
	int semid;
	key_t key;
	key=ftok(name, nr);
	if((key_t)-1==key){
		fprintf(stderr,"s_rm: bad name:<%s>\n",name); return;
	}
	semid = semget(key, 1, 0);
	if(semid<0){
		perror("s_rm:1");
		return;
	}
	if(semctl(semid,nr,IPC_RMID,0)<0){ //oznacza segment do zniszczenia po odłączeniu ostatniego procesu
		perror("s_rm:2");
		return;
	}
}
