#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, ns;
    char msg[100], reply[100];
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return 1; }
    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8083);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind"); close(s); return 1;
    }
    listen(s, 5);
    printf("Stop-and-Wait server waiting...\n");
    ns = accept(s, NULL, NULL);

    while (1) {
        int n = recv(ns, msg, sizeof(msg) - 1, 0);
        if (n <= 0) break;
        msg[n] = '\0';
        if (strcmp(msg, "END") == 0) break;

        int frame;
        if (sscanf(msg, "FRAME:%d", &frame) != 1) continue;
        printf("Received Frame %d\n", frame);
        sprintf(reply, "ACK:%d", frame);
        send(ns, reply, strlen(reply), 0);
        printf("Sent ACK %d\n", frame);
    }
    close(ns);
    close(s);
    return 0;
}
