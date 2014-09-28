
//#include "types.h"
//#include "stat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "user.h"

#define MAX_ARGSIZE 1025

int
main(int argc, char *argv[])
{

     char str[MAX_ARGSIZE];
     char * arg;
     char* cmdargs[MAX_ARGSIZE / 2];
      int count;    

    while(1 == 1)
    {
    	count = 0;

	if (fgets(str, MAX_ARGSIZE, stdin) == NULL) {
	    printf("Error!\n");
	    continue;
        }
	
        arg = strtok(str, " \t");
	while (arg != NULL && strcmp(arg, "\n") != 0) 
	{
                cmdargs[count] = arg;
		count++;

            arg = strtok(NULL, " \t");
	}
    }
}
