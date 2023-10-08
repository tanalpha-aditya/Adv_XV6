#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT1 8080 // Port for clientA
#define PORT2 8081 // Port for clientB
#define MAX_BUFFER_SIZE 1024

// Function to determine the game result
char *determineResult(int choiceA, int choiceB)
{
    if (choiceA == choiceB)
    {
        return "Draw";
    }
    else if ((choiceA == 0 && choiceB == 2) || (choiceA == 1 && choiceB == 0) || (choiceA == 2 && choiceB == 1))
    {
        return "Player A wins";
    }
    else
    {
        return "Player B wins";
    }
}

int main()
{
    int server_fdA, server_fdB, new_socketA, new_socketB;
    struct sockaddr_in server_addrA, server_addrB, client_addrA, client_addrB;
    socklen_t client_addr_lenA = sizeof(client_addrA);
    socklen_t client_addr_lenB = sizeof(client_addrB);
    char bufferA[MAX_BUFFER_SIZE] = {0};
    char bufferB[MAX_BUFFER_SIZE] = {0};
    int choiceA, choiceB;

    // Create TCP sockets for clients
    if ((server_fdA = socket(AF_INET, SOCK_STREAM, 0)) == -1 || (server_fdB = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addrA.sin_family = AF_INET;
    server_addrA.sin_addr.s_addr = INADDR_ANY;
    server_addrA.sin_port = htons(PORT1);

    server_addrB.sin_family = AF_INET;
    server_addrB.sin_addr.s_addr = INADDR_ANY;
    server_addrB.sin_port = htons(PORT2);

    // Bind sockets to respective ports
    if (bind(server_fdA, (struct sockaddr *)&server_addrA, sizeof(server_addrA)) == -1 ||
        bind(server_fdB, (struct sockaddr *)&server_addrB, sizeof(server_addrB)) == -1)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("TCP RPC Server listening for ClientA on port %d and ClientB on port %d...\n", PORT1, PORT2);

    // Listen for incoming connections
    if (listen(server_fdA, 1) == -1 || listen(server_fdB, 1) == -1)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connections
    if ((new_socketA = accept(server_fdA, (struct sockaddr *)&client_addrA, &client_addr_lenA)) == -1 ||
        (new_socketB = accept(server_fdB, (struct sockaddr *)&client_addrB, &client_addr_lenB)) == -1)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // Receive choices from ClientA and ClientB
        if (recv(new_socketA, bufferA, MAX_BUFFER_SIZE, 0) == -1 ||
            recv(new_socketB, bufferB, MAX_BUFFER_SIZE, 0) == -1)
        {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        // Convert choices to integers
        choiceA = atoi(bufferA);
        choiceB = atoi(bufferB);

        // Determine the result of the game
        char *result = determineResult(choiceA, choiceB);

        // Send the result back to ClientA and ClientB
        if (send(new_socketA, result, strlen(result), 0) == -1)
        {
            perror("Send to ClientA failed");
            exit(EXIT_FAILURE);
        }

        if (send(new_socketB, result, strlen(result), 0) == -1)
        {
            perror("Send to ClientB failed");
            exit(EXIT_FAILURE);
        }

        // Prompt for another game
        printf("Play another game? (yes/no): ");
        char responseA[MAX_BUFFER_SIZE], responseB[MAX_BUFFER_SIZE];
        if (recv(new_socketA, responseA, MAX_BUFFER_SIZE, 0) == -1 ||
            recv(new_socketB, responseB, MAX_BUFFER_SIZE, 0) == -1)
        {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        // Check if both clients want to play again
        if (strcmp(responseA, "yes") != 0 || strcmp(responseB, "yes") != 0)
        {
            // If any client says "no," continue listening for new games
            printf("Waiting for new game requests...\n");

            // You can choose to reset the game state here if needed
        }
    }

    close(new_socketA);
    close(new_socketB);
    close(server_fdA);
    close(server_fdB);

    return 0;
}
