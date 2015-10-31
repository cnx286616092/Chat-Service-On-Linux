#include "mess.h"

regex_t preg;

void mess_init(void)
{
	if ( regcomp(&preg, REGEX, REG_EXTENDED) != 0)
	{
		printf("Regcomp Railed!\n");
		exit(1);
	}
}

void mess_prce(const char* buffer,char** sendto,char** from )
{
	if(buffer == NULL  || sendto == NULL  || from == NULL)
	{
		printf("the str Pointer is NULL!\n");
	}


	regmatch_t match[REGNUM + 1];
	int err = regexec(&preg, buffer, REGNUM + 1 , match , 0);

	char str[102];
	if( err != 0 )
	{
		regerror(err,&preg,str,100);
		printf("Regexec failed!\n");
		printf("%s\n",str);
	}

	int i = 0 ;
	int j = 0 ;
	for(i = 1; i < 3 ; i++)
	{
		int len = match[i].rm_eo - match[i].rm_so ;
		char* tmp = (char*) malloc((len + 1) * sizeof(char));

		if(tmp == NULL)
		{
			printf("Str build Failed!\n");
		}
		for( j = 0 ; j < len ; j++)
		{
			tmp[j] = buffer[j + match[i].rm_so];
		}
		tmp[j] = 0;

		switch(i)
		{
		case 1: *sendto = tmp ; break;
		case 2: *from   = tmp ; break;
		}

	}

}
void mess_des(void)
{
	regfree(&preg);
}
