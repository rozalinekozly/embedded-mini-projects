/*
	parent is writer , child is reader
	pipe is one-way
	info passed from 0 to 1
	pipe is a kernel object, the kernel is the one that manages it,
	fd[0] and fd[1] are jsut a handlers/keys/identifiers
	
*/
/*parent process*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() 
{
	/*
		declare on a pipe of size 2 int (= data type)
		cell 0 is read end
		cell 1 is write end (os defined that can't be changed)
	*/
    int fd[2];
    char message[] = "Hello from Papa via exec!";
    char buffer[100];
	
	/*
		pipe() is used to define a broadcast channel on-way! between processes to 
		send information directly.
		from one process's output to other's process's input via os memory, without a 
		temporary use of a file in disc.
		when called:
		1- os creates a buffer inside "fd" array and returns it's file discriptor
		* return value: 0 = success, -1 = fail.
		* each "fd" entry gets a file discriptor = a handler to communicate with it
	*/
    if(-1 == pipe(fd))
    {
    	/*pipe creation has failed*/
    	return -1;
    }
    /*create a new process (child)*/
    int id = fork();
	
	/*case 1: it's parent process */
    if (id > 0)
     {
     	/*in linux everything is a file, so we use same api - close, write, read etc!*/
     	/*
     		- close the entery we do not want to allow flow from 
     		  it (the read cell).
     		 - why parent (writer) must close pipe 0?
     		 	it doesnt need it
     		 	prevent reading ur own data
     		 	EOF: the child (reader) knows that the father has done "writing"
    the real important reason -> when all the entries are closed if the father forgets to close it
     		 	the child will wait forever.
     	*/
        close(fd[0]);
        /* write(<dest>, <pointer to src to send>, <src len>); */
        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);
    }
    else
    {
    	/*build a container that execv can take: string of strings
    	  so it's elements are strings*/
    	  /*build a string container that can hold file descriptor entry 0 = read */
        char fd_str[10];
        /*forcefully turn the entry of reading entry to a string */
        sprintf(fd_str, "%d", fd[0]);
		
		/*build execv argument: string of strings ends with null as convention*/
        char *args[] = {"./child_program", fd_str, NULL};
        /*
    		as a child give-up on writing enrty, so close it
        */
        close(fd[1]); 
        /*execv (<file's path from process's place>, <arguments array>)*/
        /*
        	execv takes the path written in first arguemnt, go to disc and 
        	loads it (replaces old code)
        	new main gets args and send them as argv and kernel do the calculation
        	automatic to get argc value until it gets to null
        */
        execv(args[0], args);
    }
    return 0;
}
/*-----------------------------------------------------------------------------*/
/*child process*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
	argc is number of elements in argc without NULL, kernel calculates number of elements
	argv is what we send via execv
	program is what we send as first argument in execv
	argv expects to have the programs path  (why? historical reason, to allow the process to have "self-concious", used mainly for printing and helping, so mutli programs run to not hardcode print that)
*/
int main(int argc, char *argv[]) 
{
	/*
		argv contains 0 = path of program, 1 = handler for reading 
		so, first extract handler via atoi 
	*/
    int fd_to_read = atoi(argv[1]);
    /*prep a placement to have the message copied here*/
    char buffer[100];
	
	/*
		* read(<from where>, <read into>, <max number of bytes it can read at once (so no overflow 
		happens>))
		* return values: positive integer = how many successfully read bytes
							0 = writer side has closed (nothing to read)
							-1 = an error has happened
	*/
    read(fd_to_read, buffer, sizeof(buffer));
    printf("Child (after exec): %s\n", buffer);

    close(fd_to_read);
    return 0;
}


