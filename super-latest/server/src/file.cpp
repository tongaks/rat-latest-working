#include <iostream>
#include <fstream>
#include <string.h>

int main() {
	std::string line;
	
	std::ifstream file ("commands.cpp"); 
	if (file.is_open()) {
		while (std::getline(file, line)) {
			std::cout << line << std::endl; 
		} file.close();
	} else std::cout << "Unable to open the file." << std::endl;

	return 0;
}