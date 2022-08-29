#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

//Shell symbol to be printed out
#define symbol "§"

//Command argument separator
#define delimiter " "

int main(int argc, char* argv[])
{
	char* line = NULL;
	size_t len = 0;

	//Token for storing the current command argument
	char* token;

	//Array of command arguments
	char* arguments[50];

	//Stores the path for the 'cd' command if used
	char* path;

	//Stores the process ID
	pid_t pid;

	printf("picosh v0.1 (29.08.2022)\n");
	printf("Author: bigkahuna\n\n");

	while(1)
	{
		printf("%s", symbol);

		//Gets the stdin and stores it in the 'line' variable pointer
		getline(&line, &len, stdin);

		//Checks if the 'exit' command is issued
		if(strcmp("exit\n", line) == 0)
		{
			exit(0);
		}

		//Counter for keeping track of the number of arguments to be stored
		int counter = 0;

		//Parsing the file to be executed and the arguments by separating using the delimiter
		token = strtok(line, delimiter);

		while(token != NULL)
		{
			arguments[counter] = token;
			token = strtok(NULL, delimiter);
			counter++;
		}
		
		//Removing the newline character from the last argument
		strtok(arguments[counter-1], "\n");

		//Checking if the 'cd' command is issued and executing it
		if(strcmp(arguments[0], "cd") == 0)
		{
			path = arguments[1];
			chdir(path);
		}
		
		//Setting the argument counter to null
		arguments[counter] = NULL;

		//Creating a child process using fork()
		pid = fork();

		//If the child process has been created, execute the file with its arguments
		if(pid == 0)
		{
			execvp(arguments[0], arguments);
			exit(1);
		}
		
		//Wait until the child process terminates
		wait(0);
		
	}

	return 0;
}
