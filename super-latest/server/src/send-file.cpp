bool Window::SendFile(std::string fileName) {
	std::string line;

	std::ifstream file(fileName); 
	if (file.is_open()) {
	    std::string line;
	    while (std::getline(file, line)) {
        std::string cmd = "echo \"" + line + "\" >> \"" + fileName + "\"";
        std::cout << "Content cmd: " << cmd << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        SendCmd(cmd);
	    } file.close(); return true;
	} else {
    	std::cout << "Unable to open the file." << std::endl;
	}

std::cout << "Sended" << std::endl;

	return true;	
}

void Window::SendFileDialog(wxCommandEvent &event) {
	wxFileDialog openFileDialog(this, "Open file", "", "", "Any files (*.*)|*.*|PNG (.png)|*.png", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (openFileDialog.ShowModal() == wxID_CANCEL) return; 

	wxString filePath = openFileDialog.GetPath().ToStdString();
	wxString fileName = openFileDialog.GetFilename().ToStdString();

	wxString createFileCmd = "type nul >" + fileName;
	if (!SendCmd(createFileCmd.ToStdString())) {
		WarningText("Failed to send file.");
		return;
	}
  
  std::this_thread::sleep_for(std::chrono::seconds(3));

	WarningText("Sending the content...");
	if (!SendFile(fileName.ToStdString())) WarningText("Failed to send the file.");
	else WarningText("Succesfully send the file.");
}

// 	 std::string buffer;
//   const size_t bufferSize = 1024; // Adjust buffer size for optimal transfer

//   // Read and send the file in chunks
//   while (!file.eof()) {
//       buffer.resize(bufferSize);
//       file.read(&buffer[0], bufferSize);

//       size_t bytesRead = file.gcount(); // Get number of bytes actually read
//       if (bytesRead > 0) {
//           buffer.resize(bytesRead); // Resize the buffer to match the actual read size
//           std::cout << "Sending chunk of size: " << bytesRead << " bytes" << std::endl;
//           SendCmd(buffer); // Send the chunk to the client
//           std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Adjust sleep for rate control
//       }
//   }