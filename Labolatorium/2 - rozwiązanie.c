/* L2B.c */
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
 
// rozwiazanie L. Opalskiego

volatile sig_atomic_t do_exit, do_proc, cnt;
int sig1, sig2;
int T=4;
pid_t child_PID, main_PID;
int setsighandler(int sig, void (*handler) (int))
{
    struct sigaction sa;
 
    memset(&sa, 0x00, sizeof(struct sigaction));
    sa.sa_handler = handler;
//  sigaddset(&(sa.sa_mask),sig1);  sigaddset(&(sa.sa_mask),sig2);  sigaddset(&(sa.sa_mask),sig3);
    if (sigaction(sig, &sa, NULL) == -1){
        perror("sigaction"); return -1;
    }
    return 0;
}
 
void handler(int sig)
{
//  fprintf(stderr,"\n===> handler: sig=%2d\n",sig);
    if(sig==sig1){
        if(child_PID<=0 ) do_proc=1;
        else kill(child_PID,sig1);
    } else if(sig==sig2){
        if(child_PID<=0 ) do_exit=1;
        else kill(child_PID,sig2);
    }
    fprintf(stderr,"<=== PID=%lu, return from handler(%2d), do_exit=%2d,do_proc=%d, cnt=%d\n",
        (unsigned long)getpid(),sig,do_exit,do_proc,cnt);
    fflush(NULL);
}
void usage(char *prg, int nr){
    fprintf(stderr,"\nUsage:\n\n%s s1 s2\ns1 - signal to create child process\n",prg);
    fprintf(stderr,"s2 - signal terminate the youngest process\n");
    fprintf(stderr,"example use:\n %s 20 2\n",prg);
    fprintf(stderr,"SIGTSTP (^Z): 20\n");
    fprintf(stderr,"SIGINT (^C): 2\n");
    fprintf(stderr,"SIGQUIT (^\\): 3\n\n");
    if(nr>0) fprintf(stderr,"Incorrect signal s%d\n\n",nr);
    exit(EXIT_FAILURE);
}
void processargs(int argc, char *argv[]){
    if(argc!=3) usage(argv[0],0);
    sig1=atoi(argv[1]); sig2=atoi(argv[2]);
    if(setsighandler(sig1,handler)<0) usage(argv[0],1);
    if(sig2==sig1 || setsighandler(sig2,handler)<0) usage(argv[0],2);
}
int do_parent(void);
 
int do_child(){
    return do_parent();
}
int do_parent(){
    do_exit=0; do_proc=0; cnt=0; child_PID=0;
    while(!do_exit) {
    int t;
        cnt++;
        fprintf(stderr,"*** PID=%lu, cnt=%d, do_proc=%d\n",(unsigned long)getpid(),cnt,do_proc);
        t=T; while(t>0) t=sleep(t);
        if(do_proc){
            pid_t pid;
            int status;
            printf("child creation\n");
            do_proc=0;
            child_PID=fork();
            switch(child_PID){
            case -1: perror("fork"); return -1;
            case  0: /* child */
                exit(do_child());
                break;
            default:/* parent */
                while (1){
                    pid=waitpid(0, &status, 0);
                    if(pid<0){
                        if(errno==EINTR) continue;
                        perror("waitpid"); break;
                    }
                    if (WIFEXITED(status)) {
                        printf("Process nr %lu exited with code=%d\n", (unsigned long)pid,
                               WEXITSTATUS(status));
                    }
                    if (WIFSIGNALED(status)) {
                        printf("Process nr %lu got signal nr %d\n", (unsigned long)pid,
                               WTERMSIG(status));
                    }
                    if (WIFSTOPPED(status)) {
                        printf("Process nr %lu stopped by signal nr %d\n",
                               (unsigned long)pid, WSTOPSIG(status));
                    }
                    if(child_PID==pid) {
                        char buf[32];
                        if(main_PID){
                            snprintf(buf,sizeof(buf),"pstree -h -p %lu",(unsigned long)main_PID);
                        //  system(buf);
                        }
                        do_exit=1;
                        break;
                    }
                }
                child_PID=0; break;
            }
//          printf("do_proc=%d\n\n",do_proc);
        }
    }
    printf("Terminating process: PID=%lu, cnt=%d\n",(unsigned long)getpid(),cnt);
    return cnt;
}
 
int main(int argc, char *argv[])
{
    main_PID=getpid();
    processargs(argc,argv);
    exit(do_parent());
}