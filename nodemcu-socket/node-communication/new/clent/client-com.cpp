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

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    
    if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connection failed\n";
        return 1;
    }

    std::cout << "Connected to server at " << SERVER_IP << ":" << SERVER_PORT << "\n";

    while (true) {
        int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            std::cout << "[+] receive: " << buffer << '\n';
            if (strcmp(buffer, "ping") == 0) {
                send(sock, "pong", 4, 0);
                std::cout << "[!] pong was sent." << '\n';
            } else if (strcmp(buffer, "229892") == 0) {
                std::cout << "[!] shutting down." << '\n';
                if (send(sock, "ok", 2, 0) == -1)
                    std::cout << "ok failed sent." << '\n';
                else std::cout << "ok was sent." << '\n';
                break;
            }
        }
    }

    close(sock);
    return 0;
}
