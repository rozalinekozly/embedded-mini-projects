#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>

struct msgbuf { long mtype; char mtext[128]; };

int main(int argc, char *argv[]) {
    if (argc < 2) { fprintf(stderr, "Usage: %s <mtype>\n", argv[0]); return 1; }

    long  mytype = atol(argv[1]);
    key_t key    = ftok("/tmp/msgq_file", 1);
    int   msqid  = msgget(key, 0666);

    struct msgbuf msg;
    msgrcv(msqid, &msg, sizeof(msg.mtext), mytype, 0);
    printf("P%ld received: %s\n", mytype, msg.mtext);

    if (mytype == 4) msgctl(msqid, IPC_RMID, NULL);
    return 0;
}