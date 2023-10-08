#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h> // Include this for fd_set and select

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define CHUNK_SIZE 1024
#define MAX_RETRANSMISSIONS 3
#define TIMEOUT_SECONDS 2

struct Packet {
    int sequence_number;
    char data[CHUNK_SIZE];
};

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    char data_to_send[] = "This is some test data that needs to be transmitted reliably.";
    int data_len = strlen(data_to_send);
    int start = 0;
    int sequence_number = 0;

    while (start < data_len) {
        struct Packet packet;
        packet.sequence_number = sequence_number;
        int end = start + CHUNK_SIZE;
        if (end > data_len) {
            end = data_len;
        }
        memcpy(packet.data, &data_to_send[start], end - start);

        // Simulate random ACK drops
        if (rand() % 10 < 2) {
            printf("ACK for sequence %d dropped.\n", sequence_number);
        } else {
            sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr*)&server_addr, server_addr_len);
            printf("Sent sequence %d\n", sequence_number);
        }

        start = end;
        sequence_number++;

        // Simulate ACK reception
        struct Packet ack_packet;
        int retries = 0;
        while (retries < MAX_RETRANSMISSIONS) {
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(sockfd, &fds);
            struct timeval timeout;
            timeout.tv_sec = TIMEOUT_SECONDS;
            timeout.tv_usec = 0;

            if (select(sockfd + 1, &fds, NULL, NULL, &timeout) > 0) {
                recvfrom(sockfd, &ack_packet, sizeof(ack_packet), 0, NULL, NULL);
                printf("Received ACK for sequence %d\n", ack_packet.sequence_number);
                break;
            } else {
                printf("Timeout, retransmitting...\n");
                retries++;
                if (retries == MAX_RETRANSMISSIONS) {
                    printf("Max retries reached. Giving up.\n");
                    break;
                }
                sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr*)&server_addr, server_addr_len);
            }
        }
    }

    close(sockfd);
    return 0;
}
