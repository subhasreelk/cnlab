#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s;
    char domain[100], ip[100];
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) { perror("socket"); return 1; }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8082);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind"); close(s); return 1;
    }
    printf("DNS server waiting on UDP port 8082...\n");

    while (1) {
        int n = recvfrom(s, domain, sizeof(domain) - 1, 0,
                         (struct sockaddr *)&client, &len);
        if (n <= 0) continue;
        domain[n] = '\0';
        printf("Requested domain: %s\n", domain);

        if (strcmp(domain, "www.google.com") == 0)
            strcpy(ip, "142.250.183.68");
        else if (strcmp(domain, "www.yahoo.com") == 0)
            strcpy(ip, "98.137.11.163");
        else if (strcmp(domain, "www.amazon.com") == 0)
            strcpy(ip, "205.251.242.103");
        else
            strcpy(ip, "IP Not Found");

        sendto(s, ip, strlen(ip), 0, (struct sockaddr *)&client, len);
    }
    close(s);
    return 0;
}
