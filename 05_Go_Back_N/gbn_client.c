#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, choice, base = 1, i, ack;
    int total = 5, window = 3;
    char msg[100], response[100];
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return 1; }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8084);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect"); close(s); return 1;
    }

    printf("1. Normal\n2. ACK loss\n3. Frame loss\n4. Timeout (simulated)\nChoice: ");
    scanf("%d", &choice);

    while (base <= total) {
        int end = base + window - 1;
        if (end > total) end = total;
        printf("\nWindow: %d to %d\n", base, end);

        for (i = base; i <= end; i++) {
            int frame_to_send = i;
            if (choice == 3 && base == 1 && i == 2) {
                printf("Frame 2 lost (not sent); sending later frames\n");
                continue;
            }

            sprintf(msg, "FRAME:%d", frame_to_send);
            printf("Sending Frame %d\n", frame_to_send);
            send(s, msg, strlen(msg), 0);

            int n = recv(s, response, sizeof(response) - 1, 0);
            if (n <= 0) { close(s); return 1; }
            response[n] = '\0';
            sscanf(response, "ACK:%d", &ack);
            printf("Received %s\n", response);

            if ((choice == 2 || choice == 4) && base == 1 && i == 1) {
                printf("%s; will retransmit from Frame 1\n",
                       choice == 2 ? "ACK 1 lost (simulated)" : "Timeout (simulated)");
                base = 1;
                break;
            }

            if (choice == 3 && base == 1 && i == 3) {
                printf("Receiver discarded Frame 3; go back to Frame 2\n");
                base = 2;
                break;
            }

            if (ack >= base) base = ack + 1;
        }
    }

    send(s, "END", 3, 0);
    close(s);
    return 0;
}
