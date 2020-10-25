/**
 *	Felekidis Iosif
 *  4062
 *  hy345 - assignment 1
 *  
 *  main file 
 **/


#include "cs345lib.h"

volatile sig_atomic_t child_pid = 0;
char *built_in[] = {"exit","cd","test"};


int main(int argc, char **argv) {
	
	signal(SIGINT, signal_handler);
	sh_loop();

	exit(EXIT_SUCCESS);
}


void sh_loop(void) {

	char *commandline, **arguments;
	int status;

	init_promt();
	do {	

		printf("%s", promt);						// print promt
		commandline = read_commandline();			// read argument line
		arguments = split_arguments(commandline);	// split argument line into commands
		status = execute(arguments); 				// execute commands

	} while(status);								// while exit command 

	free(promt);
}

char* read_commandline(void) {

	char *line = NULL;
	size_t size = 1;
	int pos = 0;
	char c;

	line = malloc(sizeof(char));
	while((c = getchar()) != '\n') {  

		if(c == '\b') {
			if(pos > 0)
				pos--;
			continue;
		}else if(c == -1) {
			continue;
		}

		line[pos] = c;
		size++;
		line = realloc(line, size*sizeof(char));
		pos++;
	}
	line[pos] = '\0';

  	return line;
}

char** split_arguments(char *commandline) {

  	int position = 0 ,commandline_size = BUFSIZE;
  	char *token;
  	char **arguments = malloc(commandline_size*sizeof(char*));

  	token = strtok(commandline, " \t\r\n\a");
  	while(token != NULL) {
  		arguments[position] = token;
    	position++;

    	if(position >= commandline_size) {

    		commandline_size += BUFSIZE;
      		arguments = realloc(arguments, commandline_size*sizeof(char*));
      		if(arguments == NULL) {
      			perror("[split_arguments] realloc ");
      			exit(EXIT_FAILURE);
      		}
    	}

    	token = strtok(NULL, " \t\r\n\a");
  	}

  	arguments[position] = NULL;
  
  	return arguments;
}
	
int execute(char** arguments) {

	if(*arguments == NULL)	return 1;					        // no input, continue					

	for(int i=0; i<2; i++) {									// if exit or cd execute them
		if(strcmp(arguments[0],built_in[i]) == 0) {
			built_in_exec(arguments, i);
			return 1;
		}
	}

	struct Commands *commands;
	int redirections = get_redirection_count(arguments);		// get number of IO redirations

	if(redirections > 0) { 										

		if(check_input(arguments)) {						    // check for syntax errors with rediraction symbols
			fprintf(stderr, "Error: syntax error of pipe\n");
			return 1;
		}
		commands = split_commands(redirections, arguments); 	// split arguments and redirection symbols 
		
	}else {
		commands = malloc(sizeof(struct Commands));
		commands->argv = arguments;								// insert the one command to be executed
	}

	sh_launch(redirections, commands);							// execute pipe or single command
	
	for(int i=0; i<=redirections; i++) {						// free arguments
		free(commands[i].argv);
	}			
	free(commands);

	return 1;
}

int sh_launch(int redirections, struct Commands *commands) {

	int main_pid, pipe_pid, i, in, out, append, status;
	
	char *io;

	if((main_pid = fork()) == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	
	if(main_pid == 0) {						// main child execute   

		child_pid = getpid();				// set pid flag

		if(strcmp(commands[0].argv[0], built_in[2])==0) // execute build in commands that require child execution {test}  
			built_in_exec(commands[0].argv, 2);	
		
		for(i=0; i<redirections; i++) {		       // execute pipes if any found
			
			io = commands[i].IO_symbol;

			if(!strcmp(">",io)) {
				
				int fd[2];
				
				if(pipe(fd) == -1) {				// init pipe
					perror("pipe");
					exit(EXIT_FAILURE);
				}
				if((pipe_pid = fork()) == -1) {		// create child
					perror("fork");
        			exit(EXIT_FAILURE);
    			}
				if(pipe_pid == 0) {					// second child execute

					dup2(fd[1], 1);									// set fd[1] as output 
					execvp(commands[i].argv[0], commands[i].argv);  // execute	
					fprintf(stderr, "unknown command: %s\n", commands[i].argv[0]);
					exit(EXIT_FAILURE);
				}

				dup2(fd[0], 0);				     // set fd[0] as input
				close(fd[1]);					 // close write end
				close(fd[0]);					 // close read end
				waitpid(pipe_pid, &status, 0);	 // wait for childs to exit
					
			}else if(!strcmp("|", io)) {
				in = open(commands[i+1].argv[0], O_RDONLY);						// read input from a file
   				dup2(in, 0);  													// set file as input
   				close(in);
   				break;
   			}else if(!strcmp("||", io)) { 
				out = open(commands[i+1].argv[0], O_WRONLY | O_TRUNC);          // write output to file overwriting it
				dup2(out, 1);													// set file as output
				close(out);
				break;
			}else if(!strcmp("|||", io)) {	
				append = open(commands[i+1].argv[0], O_WRONLY | O_APPEND);		// append output to file
				dup2(append, 1);												// set file as output	
				close(append);
				break;
			}		
		}

		execvp(commands[i].argv[0], commands[i].argv);				            // execute command that will output at stdout
		fprintf(stderr, "unknown command: %s\n", commands[i].argv[0]);
		exit(EXIT_FAILURE);
	}

	setpgid(main_pid,main_pid);				// change gid of main child to his pid
	tcsetpgrp(STDIN_FILENO, main_pid);		// give foreground control of stdin to children
	signal(SIGTTOU, SIG_IGN);				// ignore sigttou (parent stop)
	waitpid(main_pid, &status, 0);			// main process wait for main child
	tcsetpgrp(STDIN_FILENO, getpid());		// give back stdin to parent
	child_pid = 0;							// reset pid flag 

	return 1;								// return success
}

void signal_handler(int sig) {

    char nl[] = "\n";
   
    if(sig == SIGINT) {

    	signal(SIGINT, signal_handler);			// reset signal
    	if(child_pid != 0) {					// if child active
        	kill(-child_pid, SIGKILL);			// kill all children 
    	}else {					
    		if(write(STDOUT_FILENO, nl, sizeof(nl) - 1)==-1);	
    		if(write(STDOUT_FILENO, promt, strlen(promt)) ==-1);	// print promt again
    	}
    }
}








