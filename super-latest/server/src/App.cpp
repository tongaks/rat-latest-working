#include "../headers/App.h"
#include "../headers/server.h"

bool App::OnInit() {
	Window *window = new Window("Server", wxDefaultPosition, wxDefaultSize); 
	window->Show(true);
	return true;
}

wxIMPLEMENT_APP(App);