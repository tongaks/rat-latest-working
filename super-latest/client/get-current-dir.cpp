#include <iostream>
#include <direct.h>

int main() {
	char buffer[1024];
	char s;

	if (getcwd(buffer, sizeof(buffer) / sizeof(buffer[0])) != NULL)
		std::cout << "Current dir: " << buffer << std::endl;
	else
		std::cerr << "Failed to get the current dir." << std::endl;

	std::cin >> s;
	return 0;
}