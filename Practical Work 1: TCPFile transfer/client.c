#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 12345

void send_file(const char *filename) {
    int client_socket;
    struct sockaddr_in server_addr;
    FILE *file_to_send;
    char buffer[1024];
    size_t bytes_read;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Prepare the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    // Open file to send
    file_to_send = fopen(filename, "rb");
    if (file_to_send == NULL) {
        perror("File not found");
        exit(1);
    }

    // Read file and send data
    while ((bytes_read = fread(buffer, sizeof(char), sizeof(buffer), file_to_send)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }

    printf("File sent successfully.\n");

    // Close the file and socket
    fclose(file_to_send);
    close(client_socket);
}

int main() {
    send_file("file_to_send.txt");
    return 0;
}
