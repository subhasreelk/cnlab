#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

int main() {
    int s, choice, i;
    char msg[100], ackmsg[100];
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return 1; }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8083);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect"); close(s); return 1;
    }

    printf("1. All frames transmitted\n2. ACK lost\n3. Frame lost\n4. Timeout\nChoice: ");
    scanf("%d", &choice);

    for (i = 1; i <= 4; i++) {
        int attempt = 0;
        while (1) {
            attempt++;
            if (choice == 3 && i == 1 && attempt == 1)
                printf("Simulating Frame 1 loss (not sent)\n");
            else {
                sprintf(msg, "FRAME:%d", i);
                printf("Sending Frame %d\n", i);
                send(s, msg, strlen(msg), 0);
            }

            fd_set set;
            FD_ZERO(&set);
            FD_SET(s, &set);
            struct timeval tv = {3, 0};
            int ready = select(s + 1, &set, NULL, NULL, &tv);

            if (ready == 0) {
                printf("Timeout! Retransmitting Frame %d\n", i);
                continue;
            }

            int n = recv(s, ackmsg, sizeof(ackmsg) - 1, 0);
            if (n <= 0) { close(s); return 1; }
            ackmsg[n] = '\0';

            if (choice == 2 && i == 1 && attempt == 1) {
                printf("ACK lost (sender ignores it)\n");
                continue;
            }
            if (choice == 4 && i == 1 && attempt == 1) {
                printf("Simulated timeout: ACK ignored\n");
                continue;
            }

            printf("Received %s\n", ackmsg);
            break;
        }
    }
    send(s, "END", 3, 0);
    close(s);
    return 0;
}
