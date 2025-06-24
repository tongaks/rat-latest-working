#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "new.h"

const char* ssid = "wifi_Wifi";
const char* passwd = "338171802"; 

WiFiServer wifi_server(8080);
AsyncWebServer web_server(80);
std::vector<WiFiClient> clients;

void setup() {
	Serial.begin(9600);
	Serial.println("Begin");

	WiFi.hostname("nodee");

	ConnectToWifi(ssid, passwd);
	SetupWebpages(web_server, clients);

	web_server.begin();
	wifi_server.begin();

	Serial.println("[+] Everythings is good, handling connections now.");
}

void loop() {
	WiFiClient new_client = wifi_server.available();

	if (new_client.connected()) {
		Serial.println("[+] A client is connected.");
		clients.push_back(new_client);
		clients_ip.push_back(new_client.remoteIP().toString());
	}

	HandleConnection(clients);
}