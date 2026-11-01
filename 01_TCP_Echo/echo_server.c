#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, ns;
    char msg[100];
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind"); close(s); return 1;
    }
    listen(s, 5);
    printf("Echo server waiting on port 8080...\n");

    ns = accept(s, NULL, NULL);
    if (ns < 0) { perror("accept"); close(s); return 1; }

    while (1) {
        int n = recv(ns, msg, sizeof(msg) - 1, 0);
        if (n <= 0) break;
        msg[n] = '\0';
        printf("Client: %s\n", msg);
        if (strcmp(msg, "exit") == 0) break;
        send(ns, msg, strlen(msg), 0);
    }

    close(ns);
    close(s);
    return 0;
}
