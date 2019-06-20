#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define pinbutt 14
#define pinrele 12

const char* ssid        = "XXXX";
const char* password    = "XXXX";

boolean acceso = LOW, butt = LOW, butt0 = LOW;

ESP8266WebServer server(80);

void onf() {
  if (server.arg(0) == "true") acceso = HIGH;
  else acceso = LOW;
  server.send(200, "text/plain", server.arg(0));
}

void onsf() {
  server.send(200, "text/plain", acceso ? "1" : "0");
}

void setup() {
  pinMode(pinrele, OUTPUT);
  digitalWrite(pinrele, LOW);
  pinMode(pinbutt, INPUT_PULLUP);
  butt0 = digitalRead(pinbutt);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
  server.on("/on", onf);
  server.on("/ons", onsf);
  server.begin();
}

void loop() {
  server.handleClient();
  digitalWrite(pinrele, acceso);
  butt = digitalRead(pinbutt);
  if (butt != butt0) {
    acceso = !acceso;
    butt0 = butt;
  }
}
