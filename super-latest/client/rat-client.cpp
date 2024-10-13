#include "client-functions.h"
#include <string.h>

int main() {
	std::string addr;
	std::cout << "Enter address: "; std::cin >> addr;  

	Client client;
	client.StartClient(addr);
	return 0;
}