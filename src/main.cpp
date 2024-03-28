#include <string>
#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "wifi.h"
#include "http.h"

int status = WL_IDLE_STATUS;
int led = LED_BUILTIN;

WiFiServer server(80);

void setup() {
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    Serial.begin(9600);
    // Check the wifi hardware is working
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("WiFi Module Failed!");
        exit(1);
    }
    // Connect to the wifi network
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
    }
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    Serial.print("Access the server at http://");
    Serial.println(ip);

    server.begin();
    digitalWrite(led, HIGH);
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        Serial.println("Client Connected...");
        std::string str;
        bool currentLineBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                str += c;
                Serial.write(c);
                if (c == '\n' && currentLineBlank) {
                    Request req = parseRequest(str);
                    Serial.print("Method: ");
                    // std::string converted to Arduino String
                    Serial.println(httpMethodToString(req.method).c_str());
                    Serial.print("Target: ");
                    // std::string converted to Arduino String
                    Serial.println(req.target.c_str());
                    Serial.print("Version: ");
                    // std::string converted to Arduino String
                    Serial.println(req.version.c_str());
                    for (const auto& pair : req.headers) {
                        Serial.print(pair.first.c_str());
                        Serial.print(": ");
                        Serial.println(pair.second.c_str());
                    }
                    Serial.print("Body: ");
                    // std::string converted to Arduino String
                    Serial.println(req.body.c_str());
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-type:text/html");
                    client.println();
                    client.println("<!DOCTYPE HTML>");
                    client.println("<html>");
                    client.println("Message from Nano");
                    client.println("</html>");
                    client.println();
                    break;
                }
                if (c == '\n') {
                    currentLineBlank = true;
                } else if (c != '\r') {
                    currentLineBlank = false;
                }
            }
        }
        delay(100);
        client.stop();
        Serial.println("Client Disconnected.");
    }
}
