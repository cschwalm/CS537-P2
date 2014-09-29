
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARGSIZE 1025
const char* SYSTEM_CALLS[] = {"exit", "cd", "pwd"}; 

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
	char* cmd;

	if (fgets(str, MAX_ARGSIZE, stdin) == NULL) {
	    printf("Error!\n");
	    continue;
        }
	
        arg = strtok(str, " \t\n");
	while (arg != NULL)
	{
            cmdargs[count] = arg;
            count++;
		
            arg = strtok(NULL, " \t\n");
	}
	cmd = cmdargs[0];
       
	if (strcmp(cmd, SYSTEM_CALLS[0]) == 0) 
	{
            exit(0);
	}
	else if (strcmp(cmd, SYSTEM_CALLS[1]) == 0) 
	{
            if (count == 1)
	    {
                chdir(getenv("HOME"));
	    }
	    else if (count == 2)
	    {
	        chdir(cmdargs[1]);
	    }
	    else 
	    {
	        printf("Error!\n");
		continue;
	    }
	    continue;
	}
	else if (strcmp(cmd, SYSTEM_CALLS[2]) == 0) 
	{
	    if (count != 1)
	    {
	        printf("Error!\n");
		continue;
	    }
	    printf("%s\n", getcwd(NULL, 0));
	    continue;
	}


	    
    }
    return 0;
}
