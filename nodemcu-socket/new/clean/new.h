#include "new-web.h"
std::vector<String> clients_ip;

void ConnectToWifi(const char* ssid, const char* passwd) {
	WiFi.begin(ssid, passwd);
	while (WiFi.status() != WL_CONNECTED) {
		delay(200);
	} Serial.println("[+] Connected to the wifi.");
}

void SetupWebpages(AsyncWebServer &server, std::vector<WiFiClient> &clients) {
	server.on("/", HTTP_GET, [&clients](AsyncWebServerRequest *request) {
		request->send(200, "text/html", MainPage(clients.size(), clients_ip));
	});

	server.on("/shutdown", HTTP_GET, [&clients](AsyncWebServerRequest *request) {
	    if (request->hasParam("id")) {
	        int client_id = request->getParam("id")->value().toInt();
	        clients[client_id].print("229892");
			request->send(200, "text/plain", "OK");
		}
	});

	server.on("/restart", HTTP_GET, [&clients](AsyncWebServerRequest *request) {
	    if (request->hasParam("id")) {
	        int client_id = request->getParam("id")->value().toInt();
	        clients[client_id].print("893234");
			request->send(200, "text/plain", "OK");	        	
		}
	});	

	server.on("/logoff", HTTP_GET, [&clients](AsyncWebServerRequest *request) {
	    if (request->hasParam("id")) {
	        int client_id = request->getParam("id")->value().toInt();
	        clients[client_id].print("837453");
			request->send(200, "text/plain", "OK");	        	
		}
	});	
}

void HandleConnection(std::vector<WiFiClient> &clients) {
	for (int i = 0; i < clients.size(); i++) {
		if (clients[i].connected() && clients[i].available()) {

			String msg = clients[i].readString();
			if (msg == "pong") clients[i].print("ping");
			else if (msg == "pongok") {
				Serial.println("from client: " + msg);
				clients.erase(clients.begin() + i);
			}

			Serial.println("from client: " + msg);

		} else if (clients[i].connected()) {
			clients[i].print("ping");
		}
	}
}