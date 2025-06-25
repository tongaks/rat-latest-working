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
	Serial.begin(115200);
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

	if (new_client) {
		if (new_client.connected()) {

				if (new_client.available()) {
					String msg = new_client.readStringUntil('\n');
					msg.trim();

					if (msg.startsWith("username:")) {
						String username = msg.substring(9);
						clients_names.push_back(username);
						Serial.println("[+] New client username: " + username);
					}
				}

				clients.push_back(new_client);
			}
		}

	HandleConnection(clients);
}
