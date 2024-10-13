client.exe: client-functions.h rat-client.cpp
	x86_64-w64-mingw32-g++-posix -pthread rat-client.cpp client-functions.h -o rat-client.exe -lws2_32 -static-libgcc -static-libstdc++ -std=c++11 -static