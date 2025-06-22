#include "web.h"

void ConnectToWifi(const char* ssid, const char* passwd) {
	WiFi.begin(ssid, passwd);
	while (WiFi.status() != WL_CONNECTED) {
		delay(200);
	}
}

void SetupWebpages(AsyncWebServer &server, std::vector<WiFiClient> &clients) {
	server.on("/", HTTP_GET, [&clients](AsyncWebServerRequest *request) {
		request->send(200, "text/html", MainPage(clients.size()));
	});

	// when a client connected create a div of it with the somoe id so that
	// when the client receive shutdown, the div gets remove by js  
	server.on("/shutdown", HTTP_GET, [&clients](AsyncWebServerRequest *request) {
	    if (request->hasParam("id")) {
	        int client_id = request->getParam("id")->value().toInt()-1;
	        clients[client_id].print("229892");
	        delay(500);
			request->send(200, "text/plain", "OK");
		}
	});

	server.on("/restart", HTTP_GET, [&clients](AsyncWebServerRequest *request) {
	    if (request->hasParam("id")) {
	        int client_id = request->getParam("id")->value().toInt()-1;
	        clients[client_id].print("893234");

			request->send(200, "text/plain", "OK");
		}
	});	
}

void HandleConnection(std::vector<WiFiClient> &client_list) {
	for (int i = 0; i < client_list.size(); i++) {
		if (client_list[i].connected() && client_list[i].available()) {
			String msg = client_list[i].readString();
			if (msg == "pong") client_list[i].print("ping");
		}
	}	
}