#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "192.168.1.13"  // Replace with NodeMCU's IP
#define SERVER_PORT 8080

int main() {
    int sock;
    struct sockaddr_in server;
    char buffer[1024] = {0};

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // Define server address
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    
    if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        return 1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connection failed\n";
        return 1;
    }

    std::cout << "Connected to server at " << SERVER_IP << ":" << SERVER_PORT << "\n";

    // Communication loop
    while (true) {
        int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            std::cout << "Server response: " << buffer << "\n";

            if (buffer == "229892") {
                std::cout << "server sent a shutdown signal. sending a response..." << '\n';
                send(sock, "ok", 4, 0);
            } 
        }
    }

    close(sock);
    return 0;
}
