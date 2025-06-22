void Window::StartServer(wxCommandEvent &event) {

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(5953);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int opt = 1;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	bind(server_socket, (sockaddr*) &server_address, sizeof(server_address));
	listen(server_socket, 1);

	Announcement("Socket was created.");
	Announcement("Handling incoming connections.");

	std::thread connecting_handler_obj([this]() {
		this->HandleIncomingConnection();
	});

	connecting_handler_obj.detach();
}

void Window::HandleIncomingConnection() {
	// start_button->Enable(false);

	while (true) {

		struct sockaddr_in client_info;
		socklen_t len = sizeof(client_info);
		int client_socket = accept(server_socket, (struct sockaddr*) &client_info, &len);
		char client_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &client_info.sin_addr, client_ip, INET_ADDRSTRLEN);

		{
			client_mutex.lock();
			client_sockets.push_back(client_socket);
			client_count++;

			char buffer[1024];
			recv(client_socket, buffer, sizeof(buffer), 0);

	        if (strlen(buffer) == 3) {	// check for client's id
	            AddContactToGrid(buffer, client_ip);
	        }

			client_mutex.unlock();
		}

		std::string ip(client_ip);

		std::string msg = "A client connected: " + ip;
		printf("client count: %i\n", client_count);
		Announcement(msg);

		std::thread connection_handler_obj(&Window::HandleClient, this, client_sockets[client_count-1], ip, client_count-1);
		connection_handler_obj.detach();
	}
}

void Window::HandleClient(int socket, std::string ip, int pos) {
    send(socket, "ping", 4, 0);

    while (true) {
        char buffer[1024] = {0};
        int received = recv(socket, buffer, sizeof(buffer), 0);

        if (received < 0) {
        	std::string err = "Server had a problem receiving the data from " + ip;
            Error(err);
            break;
        }

        if (received == 0) {
        	std::string err = "Client " + ip + " disconnected from the server.";
            Error(err);
            
            {
            
            clients_grid->DeleteRows(pos);
            client_mutex.lock();
            client_count--;
            client_mutex.unlock();

            }
            break;
        }

        if (received > 0 && strcmp(buffer, "ping") == 0) {
            send(socket, "pong", 4, 0);  // Send pong back
        }
    }

    close(socket);  
}
