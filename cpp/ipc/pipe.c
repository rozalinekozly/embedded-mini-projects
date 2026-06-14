/*
ping-pong via unnamed pipes, required relationship is parent-child.
used pipe to sync ping-pong prints, bc of their ability to block 
pipes are basically fds, so they are inhereted 
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ROUNDS 5 

int main()
{
    int p2c[2], c2p[2];  /* parent-child, child-parent pipes */
    // 0 = read , 1 = write
    char buf[16]; /* placement to read from/ to write to */

    // build 2 pipes in os 
    // in other words declare them as file discriptors (they are inhereted in fork)
    // and their values are saved
    pipe(p2c);
    pipe(c2p);

    //fork a child
    //case 1: it's a child
    if (fork() == 0) 
    {
        // close write in p2c and read in c2p
        close(p2c[1]);  
        close(c2p[0]);

        for (int i = 0; i < ROUNDS; i++) 
        {
            read(p2c[0], buf, sizeof(buf)); // blocking mechansim until parent writes something
            printf("PONG\n"); 
            fflush(stdout);
            write(c2p[1], "ok", 3); //write to parent ok
        }
    } 

    else 
    {
        // close write in c2p and read in p2c 
        close(p2c[0]);
        close(c2p[1]);

        for (int i = 0; i < ROUNDS; i++) 
        {
            write(p2c[1], "go", 3); //blocking mechansim until child reads msg
            printf("PING\n");
            fflush(stdout);
            read(c2p[0], buf, sizeof(buf)); //blocking mechansim until recived "read" from child
        }
    }
    return 0;
}