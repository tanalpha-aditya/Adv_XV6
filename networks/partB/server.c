#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define CHUNK_SIZE 1024

struct Packet {
    int sequence_number;
    char data[CHUNK_SIZE];
};

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(1);
    }

    printf("Server is listening...\n");

    while (1) {
        struct Packet packet;
        recvfrom(sockfd, &packet, sizeof(packet), 0, (struct sockaddr*)&client_addr, &client_addr_len);
        printf("Received sequence %d\n", packet.sequence_number);

        // Simulate random ACK drops
        if (rand() % 10 < 2) {
            printf("ACK for sequence %d dropped.\n", packet.sequence_number);
        } else {
            sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr*)&client_addr, client_addr_len);
            printf("Sent ACK for sequence %d\n", packet.sequence_number);
        }
    }

    close(sockfd);
    return 0;
}
