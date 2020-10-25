### C shell Implementation
###### Φελεκίδης Ιωσήφ 4062




- __How it works:__

	1. Prints promt with syntax: `[cs345sh][user][current_dir]~$` 
 					
    2. Reads and separates the commands that are between pipes/redirects and stores them in an array of structs. The struct has two fields. An array of strings, that stores each command, and the commands arguments. and a string that stores the pipe/redirection symbol that follows after the command.
    	```
		struct Commands {
  			char** argv;
  			char* IO_symbol;
		};
		```
    	
    3. For each command, check if there is a pipe or redirection following, change the file descriptors accordingly and then execute.     

    4. And loop again to 1. Runs until the `exit` command.

- __Functionalities supported:__

	1. Supports all system commands and 3 built-in functions:  
		 
		- `cd`   to change directory. 
        - `exit` to terminate the shell.
		- `test` runs an infinite loop, counter. To test  ctrl-s,ctrl-q flow control hotkeys and ctrl-c terminate key.
    
    2. Pipes and multipipes.
    3. Redirections and combinations with pipes.
    4. Hotkeys `ctrl-c` terminate, `ctrl-h` backspase, `ctrl-s` and `ctrl-q` flow controls.


In the cs345lib.h there is a short explanation for what each function does.
