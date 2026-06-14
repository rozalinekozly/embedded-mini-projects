/*
named pipes: ping-pong

*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_P2G "/tmp/fifo_ping2pong"
#define FIFO_G2P "/tmp/fifo_pong2ping"
#define ROUNDS 5

int main()
 {
    // this process is responsible to create the pipes (one does that)
    // use mkfifo to declare on a named pipe
    mkfifo(FIFO_P2G, 0666);
    mkfifo(FIFO_G2P, 0666);

    // open pipes in relevant permissions
    int wfd = open(FIFO_P2G, O_WRONLY);
    int rfd = open(FIFO_G2P, O_RDONLY);
    char buf[16];


    for (int i = 0; i < ROUNDS; i++) 
    {
        write(wfd, "PING", 5); //blocked if the other process has not read yet
        printf("PING\n");
        read(rfd, buf, sizeof(buf)); //blocked if the process have not write yet (that he read the msg sent)
    }
    //close pipe's openings
    close(wfd); 
    close(rfd);
    //unlink 
    unlink(FIFO_P2G); 
    unlink(FIFO_G2P);
    return 0;
}