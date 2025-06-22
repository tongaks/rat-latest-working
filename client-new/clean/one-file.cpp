#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <fcntl.h>
#include <unistd.h>

void ConnectToServer(SOCKET sock);
void HandleCommands(std::string command, SOCKET sock);

struct sockaddr_in server_info;
char hostname[256];
int length = 256;

int main() {
	WSADATA wsa;
	SOCKET client_socket = 0;

	gethostname(hostname, length);

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
	    printf("Failed. Error Code: %d\n", WSAGetLastError());
	    return 0;
	}

	server_info.sin_family = AF_INET;

	int port_number = 0;
	std::cout << "Enter port number: ";
	std::cin >> port_number;

	server_info.sin_port = htons(port_number);

	std::string ip_address = "";
	std::cout << "\n" << "Enter IP address: ";
	std::cin >> ip_address;

	server_info.sin_addr.s_addr = inet_addr(ip_address.c_str());
	
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET) {
		perror("Socket creation failed");
		WSACleanup();
		return 0;
	}

	u_long mode = 1; // 1 = non-blocking, 0 = blocking
    if (ioctlsocket(client_socket, FIONBIO, &mode) != 0) {
        std::cerr << "Failed to set non-blocking mode! Error: " << WSAGetLastError() << "\n";
        closesocket(client_socket);
        WSACleanup();
        return -1;
    }

    // create socket again if failed
	// if (client_socket == INVALID_SOCKET) {
	// 	closesocket(client_socket);
    //     client_socket = socket(AF_INET, SOCK_STREAM, 0);
	// }

	std::cout << "Done setting up. Connecting to the server." << '\n';
	ConnectToServer(client_socket);

	while (true) {
		char buffer[1024] = {0};

		int received = recv(client_socket, buffer, sizeof(buffer)-1, 0);  // Ensure space for null-termination
		buffer[received] = '\0';

		std::cout << "From server: " << buffer << '\n';

		if (received == 0) {
			std::cout << "[!] Disconnected from the server.\n";
			std::cout << "[!] Reconnecting again...";
			ConnectToServer(client_socket);
			continue;
		}

		buffer[received] = '\0';

		if (strcmp(buffer, "ping") == 0) {
			send(client_socket, "pong", 4, 0);
		} else {
			std::string command = std::string(buffer);
			HandleCommands(command, client_socket);
		}
	}

}

void HandleCommands(std::string command, SOCKET sock) {
	if (command == "229892") {
		std::cout << "Server sent a shutdown signal.\n"; // shutdown
		system("shutdown /s /t 5");
		send(sock, "ok", 3, 0);
	} else if (command == "893234") {
		std::cout << "Server sent a restart signal.\n"; // restart
		system("shutdown /r");	
		send(sock, "ok", 3, 0);
	} else if (command == "126773") {
		std::cout << "Server sent a lock signal.\n"; // lock
		ExitWindows(0, 0);		
	}
}

void ConnectToServer(SOCKET sock) {
   while (true) {
		int res = connect(sock, (struct sockaddr*)&server_info, sizeof(server_info));
		
		if (res == 0) {
			printf("socket: %i\n", sock);

			if (send(sock, hostname, length, 0) == -1) {
				std::cout << "[!] Failed to send hostname\n";
			} 

			break;

		} else {
			std::cout << "Error: " << res << '\n';
			std::cout << "Failed to connect. Trying again" << '\n';
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}