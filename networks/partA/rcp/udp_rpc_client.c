#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8081 // Replace with the server's port
#define MAX_BUFFER_SIZE 1024

int main() {
    int udp_socket;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE] = {0};
    char response[MAX_BUFFER_SIZE] = {0};

    // Create UDP socket
    if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Convert IPv4 and IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Get user's choice
        printf("Enter your choice (0 for Rock, 1 for Paper, 2 for Scissors): ");
        int choice;
        scanf("%d", &choice);

        // Send choice to server
        sprintf(buffer, "%d", choice);
        if (sendto(udp_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

        // Receive result from server
        if (recvfrom(udp_socket, response, MAX_BUFFER_SIZE, 0, NULL, NULL) == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        printf("Result: %s\n", response);

        // Prompt for another game
        printf("Play another game? (yes/no): ");
        scanf("%s", buffer);
        if (sendto(udp_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

        if (strcmp(buffer, "no") == 0) {
            break; // Exit the loop if the user says "no"
        }
    }

    close(udp_socket);

    return 0;
}
