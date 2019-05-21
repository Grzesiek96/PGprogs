/* rwaux.h */
#ifndef __DEF_RWAUXH
#define __DEF_RWAUX_H
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
 /* union semun is defined in <sys/sem.h> */
#else
union semun{ int val; struct semid_ds *buf; unsigned short *array;};
#endif

int m_creat(char *name, int nr, int size);
void m_rm(char *name, int nr);
int s_creat(char *name, int nr, int val);
int s_get(char *name, int nr);
void s_wait(int semid, int flag);
void s_post(int semid, int flag);
int s_getval(int semid);
void s_rm(char *name, int nr);
#endif
