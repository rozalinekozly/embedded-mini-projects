#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define ROUNDS 5

int main() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in me, client;
    me.sin_family = AF_INET;
    me.sin_port = htons(PORT);
    me.sin_addr.s_addr = INADDR_ANY;

    bind(fd, (struct sockaddr*)&me, sizeof(me));

    char buf[16];
    socklen_t len = sizeof(client);

    for (int i = 0; i < ROUNDS; i++) {
        recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&client, &len);
        printf("PONG\n");
        sendto(fd, "PONG", 5, 0, (struct sockaddr*)&client, len);
    }
    close(fd);
}