#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s;
    char msg[100];
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return 1; }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8081);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect"); close(s); return 1;
    }

    while (1) {
        printf("Client: ");
        if (!fgets(msg, sizeof(msg), stdin)) break;
        msg[strcspn(msg, "\n")] = '\0';
        send(s, msg, strlen(msg), 0);
        if (strcmp(msg, "exit") == 0) break;

        int n = recv(s, msg, sizeof(msg) - 1, 0);
        if (n <= 0) break;
        msg[n] = '\0';
        printf("Server: %s\n", msg);
        if (strcmp(msg, "exit") == 0) break;
    }
    close(s);
    return 0;
}
