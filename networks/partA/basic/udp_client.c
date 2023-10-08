#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int udp_socket;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Create UDP socket
    if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IPv4 and IP address from string to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Send data to server
    const char *message = "Hello from UDP client!";
    if (sendto(udp_socket, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    // Receive data from server
    socklen_t server_addr_len = sizeof(server_addr);
    if (recvfrom(udp_socket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &server_addr_len) == -1) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }

    printf("Server response: %s\n", buffer);

    close(udp_socket);

    return 0;
}
