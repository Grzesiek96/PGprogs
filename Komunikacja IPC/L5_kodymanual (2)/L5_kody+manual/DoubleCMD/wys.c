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

char buf[1024];
char dupa[1024];

write(1, "ls\n", 4);


read(0, buf, 1024);


memset(buf, 0, 1024);
write(1, "cd abc\n", 8);


read(0, buf, 1024);


write(1, "ls\n", 4);

perror("KONIEC");

return 0;
}


