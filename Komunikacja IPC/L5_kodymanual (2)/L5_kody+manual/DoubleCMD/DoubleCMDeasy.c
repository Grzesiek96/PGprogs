/* Napisa� program potok wywo�ywany nast�puj�co:
potok cmd1 cmd2 wynik
Program ten tworzy dwa procesy potomne za pomoc� polece� cmd1 oraz cmd, przekierowuj�c standardowe wyj�cie pierwszego procesu na standardowe wej�cie drugiego, oraz standardowe wyj�cie drugiego procesu do pliku (tu: wynik). Przyk�adowo, wywo�anie:
potok ls less lista
powinno dzia�a� tak, jak polecenie pow�oki (np. zsh):
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
