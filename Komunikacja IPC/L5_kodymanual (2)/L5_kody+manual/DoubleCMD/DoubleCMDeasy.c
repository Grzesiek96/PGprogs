/* Napisaæ program potok wywo³ywany nastêpuj¹co:
potok cmd1 cmd2 wynik
Program ten tworzy dwa procesy potomne za pomoc¹ poleceñ cmd1 oraz cmd, przekierowuj¹c standardowe wyjœcie pierwszego procesu na standardowe wejœcie drugiego, oraz standardowe wyjœcie drugiego procesu do pliku (tu: wynik). Przyk³adowo, wywo³anie:
potok ls less lista
powinno dzia³aæ tak, jak polecenie pow³oki (np. zsh):
ls | less > lista */

#include <stdio.h>

int main(int argc, char* argv[])
{
if(argc!= 4)
    {
    printf("Za malo argumentow!\n");
    }
char komenda[256];
int i;
for(i=0;i<256;i++)
    {
    komenda[i]='\0';
    }
sprintf(komenda, "%s | %s > %s", argv[1], argv[2], argv[3]);
system(komenda);
return 0;
}
