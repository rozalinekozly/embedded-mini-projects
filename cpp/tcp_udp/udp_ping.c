#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define ROUNDS 5

int main() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    char buf[16];
    socklen_t len = sizeof(server);

    for (int i = 0; i < ROUNDS; i++) {
        sendto(fd, "PING", 5, 0, (struct sockaddr*)&server, len);
        printf("PING\n");
        recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&server, &len);
    }
    close(fd);
}