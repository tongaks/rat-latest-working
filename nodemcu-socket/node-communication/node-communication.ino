#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include "new.h"

const char* ssid = "wifi_Wifi";
const char* passwd = "338171802"; 

WiFiServer server(8080);
std::vector<WiFiClient> clients;

void setup() {
	Serial.begin(115200);
	WiFi.begin(ssid, passwd);

	while (WiFi.status() != WL_CONNECTED) {
		delay(200);
	}

	Serial.println("Connected to the wifi");
	server.begin();
}

void loop() {
	WiFiClient new_client = server.available();

	if (new_client.connected()) {
		clients.push_back(new_client);
		new_client.print("ping");
	}

	HandleConnection(clients);

}

void HandleConnection(std::vector<WiFiClient> &client_list) {
	for (int i = 0; i < client_list.size(); i++) {
		if (client_list[i].connected() && client_list[i].available()) {
			String msg = client_list[i].readString();
			if (msg == "pong") client_list[i].print("ping");
		}
	}	
}
