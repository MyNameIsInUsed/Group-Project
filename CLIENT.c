#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "192.168.243.128"
#define SERVER_PORT 8484
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char user_input[BUFFER_SIZE];
    char server_reply[BUFFER_SIZE];

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Failed to create socket");
        return 1;
    }

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr)) <= 0) {
        perror("Invalid address or address not supported");
        return 1;
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // Get user input
    printf("Enter a string: ");
    fgets(user_input, BUFFER_SIZE, stdin);

    // Send user input to the server
    if (send(client_socket, user_input, strlen(user_input), 0) < 0) {
        perror("Send failed");
        return 1;
    }

    // Receive and print the server reply
    if (recv(client_socket, server_reply, BUFFER_SIZE, 0) < 0) {
        perror("Receive failed");
        return 1;
    }
    printf("Server reply: %s\n", server_reply);

    // Close the socket
    close(client_socket);

    return 0;
}
