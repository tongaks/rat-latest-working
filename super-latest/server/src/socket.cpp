void Window::StartServer(wxCommandEvent &event) {
	std::cout << "Server starting..." << std::endl;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(3455);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSock < 0) {
		std::cout << "Error on creating socket" << std::endl;
		WarningText("Error creating socket");
		CloseSocket();
	}
	
	int opt = 1;
	if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		WarningText("Error reusing the address");
		CloseSocket();
	}

	if (bind(serverSock, (sockaddr*) &serverAddr, sizeof(serverAddr)) != 0) {
		std::cout << "Error on binding" << std::endl;
	   	WarningText("Error on bind");
	   	CloseSocket();
	}

	if (listen(serverSock, 1) < 0) {
		std::cout << "Error on binding" << std::endl;
	} 	WarningText("Listening for connections...");

	std::thread conHandlerObj([this](){this->HandleConnection();});
	conHandlerObj.detach();

	startButton->Enable(false);
}

void Window::HandleConnection() {
	while (true) {
		int clientSock = accept(serverSock, nullptr, nullptr);
		if (clientSock < 0) {
			WarningText("Failed to accept the client.");
			continue;
		}

		{
			std::lock_guard<std::mutex> lock(clientMutex);
			clientSocks.push_back(clientSock);			
			clientCount++;
		}

	  std::cout << "[~]Client socket: " << clientSock << std::endl;
		std::cout << "[~]Client count: " << clientCount << std::endl;

		std::string clientAddr = GetClientAddr(clientSock);
		std::cout << "[+]A client is connected: " << clientAddr << std::endl << std::endl ;
		WarningText("Client connected: " + clientAddr);
		AddClient(clientAddr, clientCount);

		std::thread clientHandlerObj(&Window::HandleClient, this, clientSock, clientAddr, clientCount);
		clientHandlerObj.detach();

		std::thread sendObj([this](){CheckClientCount();});
		sendObj.join();
	}
}

void Window::HandleClient(int sock, std::string addr, int clientID) {
  std::cout << "[+]Handling the client's connection." << std::endl << std::endl;
  
  while (true) {
	  char buffer[1024];
	  int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);

	  if (bytesReceived > 0) {
      buffer[bytesReceived] = '\0'; // Null-terminate the received data
      WarningText(buffer);
      std::cout << "Received from client: " << buffer << std::endl;
	  } else if (bytesReceived == 0) {
      WarningText("Client: " + addr + " disconnected.");

			{
				std::lock_guard<std::mutex> lock(clientCountMutex);
				clientCount--;
				std::cout << "[!]Client count: " << clientCount << std::endl;

				int rowCount = clientTable->GetNumberRows();
				selectedClient = 0;
				currentSock = 0;

	      for (int i = 0; i <= rowCount; i++) {
		      std::string idStr = clientTable->GetCellValue(i , 0).ToStdString();
		      int id = std::stoi(idStr);

		      if (id == clientID) {
			      clientTable->DeleteRows(i);
			      clientSocks.erase(clientSocks.begin() + i);
		      	break;
		      }
	      }
			}

      break;
	  } else {
      perror("Error receiving data from client");
      WarningText("Error receiving data from client");
			break;
	  }
  }

  close(sock);
}

void Window::GridSelectHandler(wxGridEvent &event, wxGrid &grid) {
  int selectedRow = event.GetRow();

  std::string idStr = clientTable->GetCellValue(selectedRow, 0).ToStdString();

  if (!idStr.empty()) {
    selectedClient = std::stoi(idStr);
    currentSock = clientSocks[selectedClient];	
    std::cout << "Current socket selected: " << currentSock << std::endl;
    WarningText("Client selected: " + idStr);
  }

  event.Skip();
}

bool Window::SendCmd(std::string cmd) {
  if (selectedClient < 0 || selectedClient > clientSocks.size()) {
    WarningText("Invalid client index.");
    return false;
  }

  try {
	  std::string cmdStr = cmd;
	  int res = send(clientSocks[selectedClient - 1], cmdStr.c_str(), cmdStr.size(), 0);
	  if (res >= 0) return true;  	
  } catch (const std::exception &err) {
  	std::cout << "[!]Error: " << err.what() << std::endl;
	  WarningText("Failed to send command.");
  }

  return false;
}


void Window::SendCmdHandler(wxCommandEvent &event) {
	std::string cmd = commandField->GetValue().ToStdString();
	if (SendCmd(cmd)) WarningText("Command was sent.");
}

void Window::CheckClientCount() {
  bool buttons_enabled = false;
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (clientCount > 0 && !buttons_enabled) {
      sendButton->Enable(true);
      shutdownButton->Enable(true);
      restartButton->Enable(true);
      lockButton->Enable(true);
      closeAppsButton->Enable(true);
      sendFileButton->Enable(true);
    	buttons_enabled = true;
    } else if (clientCount <= 0 && buttons_enabled) {
      sendButton->Enable(true);
      shutdownButton->Enable(true);
      restartButton->Enable(true);    	
    	buttons_enabled = false;
    }
  }
}

void Window::AddClient(std::string addr, int id) {
	int rowCount = clientTable->GetNumberRows();

	clientTable->AppendRows(1);
	clientTable->SetCellValue(rowCount, 0, std::to_string(id));
	clientTable->SetCellValue(rowCount,  1, addr);
}

std::string Window::GetClientAddr(int sock) {
	struct sockaddr_in clientInfo;
	socklen_t clientAddrLen = sizeof(clientInfo);
	char clientAddr[INET_ADDRSTRLEN];

	getpeername(sock, (sockaddr*)&clientInfo, &clientAddrLen);
	inet_ntop(AF_INET, &(clientInfo.sin_addr), clientAddr, sizeof(clientAddr));
	
	return (std::string) clientAddr;
}

void Window::WarningText(std::string msg) {
	updatesField->AppendText("[!] " + msg + "\n");
}

void Window::CloseSocket() {
	if (serverSock != -1) {
		close(serverSock);
		serverSock = -1;
	} exit(1);
}

void Window::OnQuit(wxCloseEvent& event) {
	CloseSocket();
	std::cout << "Server closed." << std::endl;
}