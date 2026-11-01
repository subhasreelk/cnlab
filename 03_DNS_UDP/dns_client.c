#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s;
    char domain[100], ip[100];
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) { perror("socket"); return 1; }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8082);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter domain name: ");
    scanf("%99s", domain);

    sendto(s, domain, strlen(domain), 0, (struct sockaddr *)&server, len);
    int n = recvfrom(s, ip, sizeof(ip) - 1, 0,
                     (struct sockaddr *)&server, &len);
    if (n > 0) {
        ip[n] = '\0';
        printf("IP Address: %s\n", ip);
    }
    close(s);
    return 0;
}
