/**
 *	Felekidis Iosif
 *  4062
 *  hy345 - assignment 1
 *  
 *  Containes the implementation of the built-in functions  
 **/

#include "cs345lib.h"


void built_in_exec(char **arguments, int flag) {

	switch(flag) {
		case 0: exit(EXIT_SUCCESS);
		case 1: cd(arguments);
				break;
		case 2: test();
	}
}

void cd(char **arguments) {

	if(arguments[1] == NULL) {
		if(chdir(getenv("HOME")) != 0) 
			fprintf(stderr, "error finding home directory\n");	
	}else 
		if(chdir(arguments[1]) != 0) {
			fprintf(stderr, "%s: %s: %s\n", 
				arguments[0], arguments[1], strerror(errno));
		}

	char* path = get_path();
	set_promt(path);
}

void test() {

	int p;	
	if((p = fork()) == 0) {
		int a = 0;
		while(1) {
			printf("%d\n", a);
			a++;
			sleep(1);
		}
	}
	else {
		wait(NULL);
	}
}


