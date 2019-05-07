#include<stdio.h>
#include <string.h>
char* reverseString(char* s)
{
	int i;
	char tempC;
	
	size_t sLen = strlen(s);
	printf("Dlugosc napisu: %d\n", sLen);
	
	char nowy[sLen];
	int count = (int)sLen/2;
	printf("imax: %d\n", count);
	for(i=0; i<count; i++)
	{
		
		tempC = s[sLen-i-1];
		printf("%c", tempC);
		nowy[sLen-i-1] = s[i];
		nowy[i] = tempC;
	}

		
	return nowy;
}

void reverseString(char *str)
{
    /* skip null */
    if (str == 0)
    {
        return;
    }

    /* skip empty string */
    if (*str == 0)
    {
        return;
    }

    /* get range */
    char *start = str;
    char *end = start + strlen(str) - 1; /* -1 for \0 */
    char temp;

    /* reverse */
    while (end > start)
    {
        /* swap */
        temp = *start;
        *start = *end;
        *end = temp;

        /* move */
        ++start;
        --end;
    }
}

int main()
{
	
	char napis[] = "piesek";
	
	reverse_string(napis);
	printf("Napis: %s\n", napis);
	

	return 0;
}
