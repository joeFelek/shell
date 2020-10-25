/**
 *	Felekidis Iosif
 *  4062
 *  hy345 - assignment 1
 *  
 *  Header file 
 */
#define _POSIX_SOURCE

#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
#include <errno.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>

#define BUFSIZE 64

struct Commands {

	char **argv;
	char *IO_symbol;
};

char *promt;

/**
 * Shell loop, 
 * print's promt, handles input and execution
 */
void sh_loop(void);

/**
 * Initialize promt global val 
 */
void init_promt(void);

/**
 * Returns absolute path 	
 */
char* get_path(void);

/**
 * set global val promt equal to the disired promt message 	
 */
void set_promt(char* path);

/**
 * Returns the command line input stored in a string 
 */
char* read_commandline(void);

/**
 * Splits the command line input string into individual arguments 
 * removing any whitespace characters and 
 * returns the result
 */
char** split_arguments(char *commandline);

/**
 * Initialize the execution process 
 * hadles the execution flow separating built in commands and system commands
 * return 0 if exit command else 1
 */
int execute(char** arguments);

/**
 * Execute built-in function
 * return 1 on success if exit command terminate the shell 
 */
void built_in_exec(char **arguments, int flag);

/**
 * terminate shell
 */
void terminate(void);

/**
 * Change current directory to directory pointed after cd command 
 */
void cd(char **arguments);

/**
 * Return number of IO rediraction/pipe symbols found 
 */
int get_redirection_count(char** argv);

/**
 * Check if rediraction/pipe symbols syntax is correct 
 * return 1 if an error found else 0
 */
int check_input(char** argv);

/**
 * Return number of strings in the table
 */
int get_size(char** table);

/**
 * Split commands and redirection/pipe symbols
 * alocate struct, save command string into struct->argv
 * save the next rediration/pipe symbol of command in struct->io_symbol
 * return the filled struct
 */
struct Commands* split_commands(int redirection, char** arguments);

/**
 * Runs all the commands in struct->argv
 * Executes all cases of rediraction/pipes and commands that use both.
 * Reads the struct->io_symbol of each command,
 * and redirects it's input/output the appropriate way for each case of io_symbol
 */
int sh_launch(int pipe_count, struct Commands *commands);

/**
 * Recieves SIGINT signal ctrl-c and kills only all the child processes  
 */
void signal_handler(int sig);

/**
 * just a test
 */
void test(void);
