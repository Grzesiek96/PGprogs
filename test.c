/* args.c
   The program is run as follows:
	./args arg1 arg2 ... argn
   It displays:
   its name and the current working directory
   the number of arguments (n)
   if n>0, then also it outputs in separate lines information about each argument.
   Content of the argument information lines:
   	position of argument (0,1,...),
	the argument
	if the argument is a name of an env. variable => value of the variable
	if the argument is a number: numerical value of the argument
	
	Additionally the program calculates the sum of squares of all 
	numeric values found. Before exiting the square root of the sum 
	is output to the standard output. 
	The exit code is to be equal to the number of arguments.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#ifndef PATH_MAX
#define PATH_MAX 128
#endif

#define ERR(source,code) (perror(source),\
		     fprintf(stderr,"%s:%d\n",__FILE__,__LINE__),\
		     exit(code))

int main(int argc, char *argv[]){
char buf[PATH_MAX];
int n;
double sum=0e0;
	/* get current directory */
	if(getcwd(buf,sizeof(buf))==NULL){
		ERR("getcwd",0);
	}
	printf("%s: CWD:<%s>\n",argv[0],buf);
	printf("argument count: %d\n",argc-1);
	
	for(n=1; n<argc; n++){
	char *p;
	double v;
		printf("arg. %d : %s",n,argv[n]);
		p=getenv(argv[n]);
		if(p){
			printf("; env. variable; value:%s\n",p);
		} else {
			if(sscanf(argv[n],"%lf",&v)==1){
				printf("; num. value: %lg\n",v);
				sum=sum+v*v;
			} else if(errno!=0){
				perror("sscanf");
			} else
				printf("\n");
		}
	}
	printf("sum=%lf, sqrt(sum)=%lf\n",sum,sqrt(sum));
	return argc-1;
}