#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, ns, frame;
    int received[10] = {0};
    char msg[100], ack[100];
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return 1; }
    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8085);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind"); close(s); return 1;
    }
    listen(s, 5);
    printf("Selective Repeat server waiting...\n");
    ns = accept(s, NULL, NULL);

    while (1) {
        int n = recv(ns, msg, sizeof(msg) - 1, 0);
        if (n <= 0) break;
        msg[n] = '\0';
        if (strcmp(msg, "END") == 0) break;

        if (sscanf(msg, "FRAME:%d", &frame) != 1) continue;
        if (frame >= 1 && frame <= 5) {
            received[frame] = 1;
            printf("Accepted and buffered Frame %d\n", frame);
            sprintf(ack, "ACK:%d", frame);
            send(ns, ack, strlen(ack), 0);
        }
    }

    printf("Frames received: ");
    for (int i = 1; i <= 5; i++)
        if (received[i]) printf("%d ", i);
    printf("\n");

    close(ns);
    close(s);
    return 0;
}
