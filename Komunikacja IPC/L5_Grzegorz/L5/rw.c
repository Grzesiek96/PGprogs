/* rw.c  The program shows a solution of the readers/writers problem.
 * It uses a file $HOME/rw.txt for commumication between writeres and readers.
 * By default "less" is invoked to view the file and "mousepad" to edit.
 * These can be redefined by environment variables: READER and WRITER/
 * If the file  $HOME/rw.txt does not exist the first rw call creates 
 * it and also a shared memory segment and 2 semaphores are created
 * and initialized. Subsequent rw calls attach the memory segment and 
 * semaphores to the process.
 * Make the program with: gcc -o rw rw.c rwaux.c
 * 
 * LJO. 2016-05-05. 
*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "rwaux.h"

struct shmseg_st{
	int count;
} *shm_seg;
#define SHM_SIZE sizeof(struct shmseg_st)
#define reader_count (shm_seg->count)

/* default viewer and editor; they can be also set by
 * environment variables: READER  and WRITER
 */
#define DEF_VIEWER	"less"
#define DEF_EDITOR  "mousepad"   //"gedit";

int usage(char *cmd, int retcod){
	if(errno) perror(cmd?cmd:"c1");
	fprintf(stderr,"Usage: rw [out_file]\n\n");
	return retcod;
}


void reader(int sem_readers, int sem_writers, char *fname){
	char *p, line[128];
	p=getenv("READER");//Zwraca wartość określonej zmiennej środowiskowej
	if(!p) p=DEF_VIEWER;
	snprintf(line,sizeof(line),"%s %s",p,fname);
	fprintf(stderr,"%s\n",line);

	s_wait(sem_readers,0);
	reader_count++;
	if(reader_count==1) 
		s_wait(sem_writers,0);
	s_post(sem_readers,0);
	system(line);
	s_wait(sem_readers,0);
	reader_count--;
	if(reader_count==0) 
		s_post(sem_writers,0);
	s_post(sem_readers,0);
}
void writer(int sem_writers, char *fname){
	char *p, line[128];
	p=getenv("WRITER");
	if(!p) p=DEF_EDITOR; 
	snprintf(line,sizeof(line),"%s %s",p,fname);
	fprintf(stderr,"%s\n",line);
	s_wait(sem_writers,0);
	system(line);
	s_post(sem_writers,0); //zwalnianie semafora
}
int main(int argc, char *argv[]){
	char fname[128], *homedir, *sem_name, *shm_name;
	int out, sem_readers, sem_writers, shmid;
	int timeout=0;
	homedir=getenv("HOME");
	if(!homedir) homedir=".";
	sem_name=homedir; shm_name=homedir;
	if(argc>1 && strlen(argv[1])>0){
		strncpy(fname,argv[1],sizeof(fname));
	} else {
		snprintf(fname,sizeof(fname),"%s/rw.txt",homedir);
	}
	if(argc>2){
		timeout=atoi(argv[2]);
		if(timeout<0) timeout=0;
	}
	if ((out = open(fname, O_RDWR|O_CREAT|O_EXCL,0660)) == -1) {
		if(errno!=EEXIST){ perror("open"); return usage("rw:open1",1); }
		if ((out = open(fname, O_RDWR)) == -1) {
			fprintf(stderr, "\n%s cannot open input file (%s)\n",
				argv[0],fname);
			return usage("rw:open2",2);
		}
		sem_readers= s_get(sem_name,1);
		sem_writers= s_get(sem_name,2);
		if(-1==(shmid=m_creat(shm_name,3, SHM_SIZE))){
			fprintf(stderr, "m_creat failed\n");
			exit(EXIT_FAILURE);
		}    
		if(sem_readers<0 || sem_writers<0 || shmid<0) exit(2);
	} else {/* IPC initialization needed */
		sem_readers= s_creat(sem_name,1,1);
		sem_writers= s_creat(sem_name,2,1);
		if(sem_readers<0 || sem_writers<0) exit(3);
		if(-1==(shmid=m_creat(shm_name,3, SHM_SIZE))){
			fprintf(stderr, "m_creat failed\n");
			exit(4);
		}
	}
	close(out);
	if((shm_seg = shmat(shmid, (void *)0, 0)) == (struct shmseg_st *)-1){
		perror("shmat"); exit(5);
	}
	while(1){
		char line[128];
		fprintf(stderr,"readers=%d, writers=%d, count=%d\n",
			s_getval(sem_readers), s_getval(sem_writers),reader_count);
		fprintf(stderr,"\nR, W, Q, X> ");
		if(fgets(line,sizeof(line),stdin)==NULL) break;
		switch(toupper(line[0])){
			case 'R': /* reader role */
				reader(sem_readers, sem_writers, fname); break;
			case 'W': /* writer role */
				writer(sem_writers, fname); break;
			case 'Q': /* quitting the program */
				exit(0);
				case 'X': /* removal of IPC objects before exiting the program */
				if(shmdt(shm_seg) == -1) perror("shmdt");
				s_rm(sem_name,1);
				s_rm(sem_name,2);
				m_rm(shm_name,3);
				if(unlink(fname)) perror("rw.txt");
				else fprintf(stderr,"rw.txt removed\n");
				return 0;
				break;
			default:
				fprintf(stderr,"Unrecognized command:<%s>\n",line);
			break;
		}
	}
	return 0;
}
