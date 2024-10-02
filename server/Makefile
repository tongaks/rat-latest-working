rat-server: App.o server.o
	g++ App.o server.o -o rat-server `wx-config --cxxflags --libs`

App.o: headers/App.h src/App.cpp
	g++ -c src/App.cpp `wx-config --cxxflags --libs`

server.o: headers/server.h src/server.cpp
	g++ -c src/server.cpp `wx-config --cxxflags --libs`

clean:
	rm *.o