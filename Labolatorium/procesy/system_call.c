/* system_call.c - runs a shell command (read from stdin) in a subprocess
*/
#include <stdlib.h>
#include <stdio.h>
int main(void){
    char buf[512];
    if(fgets(buf, sizeof(buf), stdin)!=NULL){
		int ret;
		ret=system(buf);
		if(ret)fprintf(stderr,"ret=%d\n",ret);
		return ret;
    }
   return 0;
}
