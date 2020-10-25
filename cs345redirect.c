/**
 *	Felekidis Iosif
 *  4062
 *  hy345 - assignment 1
 *  
 *  Contains the implementation of the functions that handle redirections/pipes 
 **/

#include "cs345lib.h"


int get_redirection_count(char** argv) {

	char **p = argv;
	int number_of_pipes = 0;
	char *io[4] = {">", "|", "||", "|||"};
	
	while(*p != NULL) {
		for(int i=0; i<4; i++) 
			if(strcmp(*p, io[i]) == 0) 
				number_of_pipes++;
		p++;
	}
	return number_of_pipes;
} 

int check_input(char** argv) {

	int size = get_size(argv), i,j,q;
	char *io[4] = {">", "|", "||", "|||"};

	for(i=0; i<4; i++) 
		if(!strcmp(argv[0],io[i]) || !strcmp(argv[size-1],io[i]))	// case: > ... or ... >
			return 1;	 											
	
	for(i=0; i<size-1; i++) 											// case: ... > > ...
		for(j=0; i<4; i++) 												// case: ... > | ...
			for(q=0; q<4; q++)											
				if(!strcmp(argv[j],io[i]) && !strcmp(argv[q],io[i]))  	
					return 1;											
		
	return 0;
}

int get_size(char** table) {

	char **p = table;
	int size = 0;

	while(*p!=NULL) {
		size++;
		p++;
	}
	return size;
}

struct Commands* split_commands(int redirections, char** arguments) {

	int argv_pos = 0, cmd_pos = 0;
	int	size = get_size(arguments);
	int	command_count = size-(2*redirections);	//max number of a args between pipes
	char *io_symbols[4] = {">", "|", "||", "|||"};


	struct Commands *commands = malloc((redirections+1)*(sizeof *commands)); // allocate struct 
	if(commands == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	for(int i=0; i <= redirections; i++) {	// allocate argv, io symbol
		commands[i].argv = malloc(command_count*sizeof(char*));			
		if(commands[i].argv == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		commands[i].IO_symbol = malloc(sizeof(char));
	}
	
	char **argv = arguments;

	while(*argv != NULL) {				// split arguments

		commands[cmd_pos].argv[argv_pos] = *argv;

		argv_pos++;
		argv++;
		for(int i=0; i<4; i++) {
			if(*argv != NULL && (strcmp(*argv, io_symbols[i]) == 0)) {		
				commands[cmd_pos].argv[argv_pos] = NULL;
				argv_pos = 0;
				commands[cmd_pos].IO_symbol = realloc(commands[cmd_pos].IO_symbol,sizeof(char)*(strlen(*argv) + 1));
				commands[cmd_pos].IO_symbol = *argv;
				cmd_pos++;
				argv++;
			}
		}	
	}
	commands[cmd_pos].argv[argv_pos] = NULL;


	return commands;		
}
