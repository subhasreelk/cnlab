#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, choice, i, ack;
    int total = 5, received[10] = {0};
    char msg[100], response[100];
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return 1; }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8085);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect"); close(s); return 1;
    }

    printf("1. Normal\n2. ACK loss\n3. Frame loss\n4. Timeout (simulated)\nChoice: ");
    scanf("%d", &choice);

    for (i = 1; i <= total; i++) {
        if (choice == 3 && i == 2) {
            printf("Frame 2 lost (not sent)\n");
            continue;
        }

        sprintf(msg, "FRAME:%d", i);
        printf("Sending Frame %d\n", i);
        send(s, msg, strlen(msg), 0);

        int n = recv(s, response, sizeof(response) - 1, 0);
        if (n <= 0) { close(s); return 1; }
        response[n] = '\0';
        sscanf(response, "ACK:%d", &ack);

        if (choice == 2 && i == 2) {
            printf("ACK 2 lost (simulated)\n");
            continue;
        }
        if (choice == 4 && i == 2) {
            printf("Timeout for Frame 2 (simulated)\n");
            continue;
        }

        received[ack] = 1;
        printf("Received %s\n", response);
    }

    for (i = 1; i <= total; i++) {
        if (!received[i]) {
            printf("Retransmitting only Frame %d\n", i);
            sprintf(msg, "FRAME:%d", i);
            send(s, msg, strlen(msg), 0);

            int n = recv(s, response, sizeof(response) - 1, 0);
            if (n <= 0) { close(s); return 1; }
            response[n] = '\0';
            sscanf(response, "ACK:%d", &ack);
            received[ack] = 1;
            printf("Received %s\n", response);
        }
    }

    send(s, "END", 3, 0);
    close(s);
    return 0;
}
