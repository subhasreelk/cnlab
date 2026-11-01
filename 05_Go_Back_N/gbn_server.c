#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, ns, expected = 1, frame;
    char msg[100], ack[100];
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return 1; }
    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8084);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind"); close(s); return 1;
    }
    listen(s, 5);
    printf("Go-Back-N server waiting...\n");
    ns = accept(s, NULL, NULL);

    while (1) {
        int n = recv(ns, msg, sizeof(msg) - 1, 0);
        if (n <= 0) break;
        msg[n] = '\0';
        if (strcmp(msg, "END") == 0) break;

        if (sscanf(msg, "FRAME:%d", &frame) != 1) continue;

        if (frame == expected) {
            printf("Accepted Frame %d\n", frame);
            sprintf(ack, "ACK:%d", frame);
            expected++;
        } else {
            printf("Discarded Frame %d; waiting for Frame %d\n", frame, expected);
            sprintf(ack, "ACK:%d", expected - 1);
        }
        send(ns, ack, strlen(ack), 0);
    }
    close(ns);
    close(s);
    return 0;
}
