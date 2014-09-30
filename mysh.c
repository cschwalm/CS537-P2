#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGSIZE 1025
const char* SYSTEM_CALLS[] = {"exit", "cd", "pwd"}; 

int
main(int argc, char *argv[])
{

    char str[MAX_ARGSIZE];
    char* arg;
    char* cmdargs[MAX_ARGSIZE / 2];
    int count; 
    int pid;
    int fp;

    while(1 == 1)
    {
    	count = 0;
	char* cmd;
	fp = 1;   

	if (fgets(str, MAX_ARGSIZE, stdin) == NULL)
	{
	    printf("Error!\n");
	    continue;
        }
	
        arg = strtok(str, " \t\n");
	while (arg != NULL)
	{
            if (strcmp(arg, ">") == 0)
	    {
	        arg = strtok(NULL, " \t\n");
		if (arg == NULL)
		{
		    fprintf(stderr, "Error!\n");
		    continue;
		}

	        fp = open(arg, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	    }
	    else if (strcmp(arg, ">>") == 0)
	    {
	       arg = strtok(NULL, " \t\n");
		 
	       if (arg == NULL)
	       {
	          fprintf(stderr, "Error!\n");
		  continue;
	       }
		  
	       fp = open(arg, O_APPEND | O_WRONLY);
	    }	
	    else
	    {
	       cmdargs[count] = arg;
               count++;
	    }

	    if (fp == -1)
	    {
	       fprintf(stderr, "Error!\n");
	       continue;
	    } 
		
            arg = strtok(NULL, " \t\n");
            cmd = cmdargs[0];
            cmdargs[count] = NULL;

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
	
	//Execute a different process
	//
	pid = fork();
	
	//Execute the program (this is the child process
	if(pid == 0)
	{
	   if (dup2(fp, 1) == -1)
	   {
	      fprintf(stderr, "Error!\n");
	      exit(0);
	   }
	   /* Close the file pointer if a copy has been made. */
	   if (fp != 1)
	       close(fp);
	    
           if (execvp(cmdargs[0], cmdargs) == -1)
	   {
	      printf("Error!\n");
	      //Exit the child process if there is an error
	      exit(0);
	   }
	}
	//Error forking. Print error and continue
	else if (pid == -1) 
	{
	   printf("Error!\n");
	   continue;
	}
	//Parent process. Wait for child to return, then continue execution
	else 
	{
	   if (wait(NULL) == -1)
	   {
	      printf("Error!\n");
	      continue;
	   }
	   
           continue;
	}   
    }
    return 0;
}
