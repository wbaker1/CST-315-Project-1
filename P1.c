//Code for Project 1 - CST 315 - Wesley Baker and Adam Ringwell

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER 1000

char *inputCommand(){
	char *command = NULL; //Establish our command character variable
	size_t size = 0;
	printf("Please input a command or input keycode CTRL + C to exit the program: ");
	getline(&command, &size, stdin); //Read input command
	return command;
}

char **parseInput(char *line){ //Function to parse command.
	int buffer = BUFFER; //Set buffer variable to be our BUFFER defined above.
	int pos = 0;
	char **tokens = malloc(buffer *sizeof(char*)); //Set pointer tokens that allocates memory for our command input.
	char *token;
	token = strtok(line, " \n"); //Set first token to the first string in our line
	while(token != NULL){ // Go through the entire string, for each string seperated
		tokens[pos] = token; // by whitespace. Reallocate memory for each
		pos++; //additional string
		if(pos >= buffer){
			buffer += BUFFER;
			tokens = realloc(tokens, buffer * sizeof(char*));
			if(!tokens){
				printf("Error allocating memory with malloc \n");
				exit(0);
			}
		}
		token = strtok(NULL, " \n");
	}
	tokens[pos] = NULL;
	return tokens;	//returns our parsed string.	
}

int execute(char **arguments){ //takes in our argument that derives from our parsedInput method.
	int pid, waitPid, argumentProgress;
	
	pid = fork(); //Process id is the return value of fork(); (Makes child processes from the parent process
	
	if(pid == 0){ // Execute the argument in the child process.
		if(execvp(arguments[0], arguments) == -1)//If the command cannot be interpretted display an error.
		perror("Error with execvp \n");
	}
	else if(pid < 0) //PID should be 0 at this point, if not an error has occured.
		perror("Error with PID < 0\n");
	else{
		do{ //Wait for child process to end, 
			waitPid = waitpid(pid, &argumentProgress, WUNTRACED);
		}
		while(!WIFEXITED(argumentProgress) && !WIFSIGNALED(argumentProgress)); //Exit function.
	}
	return 1;
}

int main(){
	char *command = NULL;
	char **arguments;
	int argumentProgress;
	
	do{
		command = inputCommand();
		arguments = parseInput(command);
		argumentProgress = execute(arguments);
		
		}
		while(argumentProgress);
}