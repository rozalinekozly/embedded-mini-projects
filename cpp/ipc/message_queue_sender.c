#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>

struct msgbuf {
    long mtype;
    char mtext[128];
};

int main() {
    key_t key  = ftok("/tmp/msgq_file", 1);
    int   msqid = msgget(key, IPC_CREAT | 0666);

    struct msgbuf msg;

    for (long type = 2; type <= 4; type++) {
        msg.mtype = type;
        snprintf(msg.mtext, sizeof(msg.mtext),
                 "Hello from P1 to P%ld", type);
        msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
        printf("P1 sent to type %ld\n", type);
    }
    printf("P1 done - exiting\n");
    return 0;  
}