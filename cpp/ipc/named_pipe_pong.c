#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//these are defined distenations agreed in both processes
#define FIFO_P2G "/tmp/fifo_ping2pong"
#define FIFO_G2P "/tmp/fifo_pong2ping"
//enum
#define ROUNDS 5

int main() 
{
    //assuming the other process created the pipes (mkfifo)
    //open them with relevant permissions
    int rfd = open(FIFO_P2G, O_RDONLY); //open the 
    int wfd = open(FIFO_G2P, O_WRONLY);
    char buf[16];

    for (int i = 0; i < ROUNDS; i++) 
    {
        read(rfd, buf, sizeof(buf));
        printf("PONG\n");
        write(wfd, "PONG", 5);
    }
    //close pipe's openings (this process is not the owner of the pipe so 
    // it does not take responsibility in unlinking them)
    close(rfd); 
    close(wfd);
    return 0;
}