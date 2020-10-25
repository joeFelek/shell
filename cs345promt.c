/**
 *	Felekidis Iosif
 *  4062
 *  hy345 - assignment 1
 *  
 *  promt handling  
 **/

#include "cs345lib.h"

char* get_path(void) {

	int size = BUFSIZE;
	
	char *path = malloc(size*sizeof(char*));
	if(path == NULL) {
		perror("[get_path] malloc");
		exit(EXIT_FAILURE);
	}
	
	while(getcwd(path, size) == NULL) {

		size += BUFSIZE;
        path = realloc(path, size);
        if(path == NULL) {
        	perror("[get_path] realloc");
        	exit(EXIT_FAILURE);
        }
   	}
   	
   	return path;
}

void init_promt(void) {
	promt = malloc(sizeof(char));
	if(promt == NULL) {
		perror("promt");
		exit(EXIT_FAILURE);
	}
	char *path = get_path();
	set_promt(path);	
}

void set_promt(char* path) {

	size_t path_len = strlen(path);
	char *user = getlogin();
	size_t user_len = strlen(user);

	promt = realloc(promt, path_len + user_len + 16);
	if(promt == NULL) {
		perror("promt");
		exit(EXIT_FAILURE);
	}
	sprintf(promt, "[cs345sh][%s][%s]~$", user, path);
	free(path);
}