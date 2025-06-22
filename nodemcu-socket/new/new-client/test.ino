#include <ESP8266WiFi.h>

const char *ssid = "MyNodeMCU_AP";  // Name of the WiFi network
const char *password = "12345678";  // Password (must be at least 8 characters)

void setup() {
    Serial.begin(115200);
    
    // Start the Access Point
    WiFi.softAP(ssid, password);
    
    Serial.println("Access Point Started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());  // Print the AP's IP address
}

void loop() {
    // Nothing needed here for a simple AP
}