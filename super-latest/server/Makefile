rat-server: App.o new-ui.o
	g++ App.o new-ui.o -o rat-server `wx-config --cxxflags --libs`

App.o: headers/App.h src/App.cpp
	g++ -c src/App.cpp `wx-config --cxxflags --libs`

new-ui.o: headers/server.h src/new-ui.cpp
	g++ -c src/new-ui.cpp `wx-config --cxxflags --libs`

clean:
	rm *.o