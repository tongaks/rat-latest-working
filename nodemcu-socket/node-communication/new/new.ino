#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "new.h"

const char* ssid = "wifi_Wifi";
const char* passwd = "338171802"; 
int client_count = 0;

WiFiServer wifi_server(8080);
AsyncWebServer web_server(80);
std::vector<WiFiClient> clients;

void setup() {
	Serial.begin(115200);

	ConnectToWifi(ssid, passwd);
	SetupWebpages(web_server, clients);

	web_server.begin();
	wifi_server.begin();
}

void loop() {
	WiFiClient new_client = wifi_server.available();

	if (new_client.connected()) {
		clients.push_back(new_client);
		new_client.print("ping");
	}

	HandleConnection(clients);

}