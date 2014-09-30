////////////////////////////////////////////////////////////////////////////////
// CS 537 - Project 2a (Fall 2014)
// Partner 1: Corbin Schwalm <schwalm@wisc.edu> : schwalm
// Partner 2: Zachary York <zyork@wisc.edu> : zyork
////////////////////////////////////////////////////////////////////////////////

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

/* Function Decelarations. */
int getCallForPipe(char* str, int* size, char** cmds, int* size2, char** cmds2);
int getCallForRedirection(char* str, int* size, char** cmds, int* fd);

int
main(int argc, char *argv[])
{
	while (1 == 1)
	{

		char str[MAX_ARGSIZE];   
		char* cmdargs[MAX_ARGSIZE / 2];
		char* cmdargs2[MAX_ARGSIZE / 2];
		char* tempLine;
		int count = 0, count2 = 0;
		int fd = 1;
		int pid;
		int returnValue;
		int pipeFlag = 0;
		int p[2];

		if (fgets(str, MAX_ARGSIZE, stdin) == NULL)
		{
			printf("Error!\n");
			continue;
		}

		if (strchr(str, '|') != NULL)
		{
			returnValue = getCallForPipe(str, &count, cmdargs, &count2, cmdargs2);
			pipeFlag = 1;
		}
		else if (strchr(str, '>') != NULL)
		{
			returnValue = getCallForRedirection(str, &count, cmdargs, &fd);
		}
		else
		{
			tempLine = strtok(str, " \t\n");

			while (tempLine != NULL)
			{
				cmdargs[count] = tempLine;
				cmdargs[count + 1] = NULL;
				count++;

				tempLine = strtok(NULL, " \t\n");
   		}
		}

		if (returnValue == -1)
		{
			printf("Error!\n");
			continue;
		}

		//Input has been parsed.

		if (strcmp(cmdargs[0], SYSTEM_CALLS[0]) == 0) 
		{
			exit(0);
		}
		else if (strcmp(cmdargs[0], SYSTEM_CALLS[1]) == 0) 
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
			}
			continue;
		}
		else if (strcmp(cmdargs[0], SYSTEM_CALLS[2]) == 0) 
		{
			if (count != 1)
			{
				printf("Error!\n");
				continue;
			}
			printf("%s\n", getcwd(NULL, 0));
			continue;
		}

		//Check To See If Pipe Needs To Be Created
		if (pipeFlag == 1)
		{
			pipe(p);
			fd = p[1];
		}

		//Create a different process
		pid = fork();

		//Execute the program (this is the child process)
		if(pid == 0)
		{

			if (pipeFlag == 1)
			{
				close(p[1]);
				dup2(p[0], 0);
			}
			else
			{
				if (dup2(fd, 1) == -1)
				{
					fprintf(stderr, "Error!\n");
					exit(0);
				}
			}

			//Close the file pointer if a copy has been made.
			if (fd != 1 && pipeFlag == 0)
				close(fd);

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
			if (pipeFlag)
			{
				close(p[0]);
				dup2(p[1], 1);
			}

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

//
// Accepts a string of characters and sets the last four parameters of
// the of this function call. It uses the supplied arrays
// and correctly sets the size parameters. This is used for the pipe special
// case.
//
// Returns -1 if the data was invalid.
//
int
getCallForPipe(char* str, int* size, char** cmds, int* size2, char** cmds2)
{
   str = strtok(str, " \t\n");
	
   /* Fill array 1. */   
   while (str != NULL)
   {
      if (strcmp(str, "|") == 0)
		{
         /* End of section 1. */
         str = strtok(NULL, " \t\n");
         break;
		}

      cmds[*size] = str;
      cmds[*size + 1] = NULL;
      (*size)++;

      str = strtok(NULL, " \t\n");
   }

   /* Valid Data Check. */
   if (str == NULL)
   {
      return -1;
   }

   /* Fill array 2. */   

   while (str != NULL)
   {
      cmds2[*size2] = str;
      cmds2[*size2 + 1] = NULL;
      (*size2)++;

      str = strtok(NULL, " \t\n");
   }

   return 0;
}

//
// Accepts a string of characters and sets the passed array with its 
// argument tokens. The size value is also correctly set.
//
// The output parameter is used to set the file descripter to redirect the output.
//
// If the supplied string is invalid, -1 is returned.
//
int
getCallForRedirection(char* str, int* size, char** cmds, int* fd)
{
   str = strtok(str, " \t\n");

   while (str != NULL)
   {
      if (strcmp(str, ">") == 0)
		{
         str = strtok(NULL, " \t\n"); //Advance to the file name.         
         *fd = open(str, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
         break;
		}
      else if (strcmp(str, ">>") == 0)
      {
         str = strtok(NULL, " \t\n"); //Advance to the file name.         
         *fd = open(str, O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
         break;
      }

      cmds[*size] = str;
      cmds[*size + 1] = NULL;
      (*size)++;

      str = strtok(NULL, " \t\n");
   }

   return *fd;
}
