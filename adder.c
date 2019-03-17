#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUF_MAX 128
//bez zmian
int main(int argc,char* argv[]){
	char buf[BUF_MAX];
	char *p;
	int sum =0;
	
	getcwd(buf, BUF_MAX);
	printf("PATH %s \n", buf);
	
	printf("Nazwa: %s, liczba argumentów: %d \n", 
	argv[0], argc-1); 
	
	int i;
	
	for(i=1; i< argc; i++){
		
		p = getenv(argv[i]);
		
		if(p)
			printf("Zmienna środowiskowa: %s wartosc %s \n",argv[i], p);
		else
			printf("Zmienna [%d]: %s \n",i, argv[i]);
			printf("Suma %d \n", sum);
	}
			
	
	
	//*p = getenv(argv);
	//printf("%s", *p);
}
