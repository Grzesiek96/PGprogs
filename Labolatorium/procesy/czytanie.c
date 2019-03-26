/* czytanie.c - ilustracja wykorzystania funkcji strtok
 * do podzialu linii, wczytanej za pomoca funkcji fgets,
 * na wyrazy. Separatory wyrazów zdefiniowano w sep.
 */
#include <stdio.h>
#include <string.h>
#define BUFLEN	80
int main(int argc, char *argv[]){
char buf[BUFLEN];
char *sep=" \t\n";
	while(fgets(buf,BUFLEN,stdin)!=NULL){
	char *p1, *p2;
	int len;
		len=strlen(buf);
		if(buf[len-1]=='\n') buf[len-1]='\0';
		printf("linia (len=%d):<%s>\n",len,buf);
		p1=buf;
		while((p2=strtok(p1,sep))!=NULL){
			printf("wyraz: %s\n",p2);
			p1=NULL;
		}
	}
	return 0;
}
