#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <HSBColor.h>

#define pinR 14   // Pin D5 
#define pinG 12   // Pin D6
#define pinB 13   // Pin D7

const char* ssid        = "XXXX";
const char* password    = "XXXX";

int colore [3] = {0, 0, 0}, colorev [3] = {0, 0, 0}, hue = 0, sat = 0, bri = 100;
boolean acceso = LOW, att=LOW;
unsigned int tatt = 0;

ESP8266WebServer server(80);

void onf() {
  if (server.arg(0) == "true") {
    acceso = HIGH;
    sat = 100;
    if (millis() - tatt <= 1000) att = HIGH;
    tatt = millis();
  }
  else acceso = LOW;
  server.send(200, "text/plain", server.arg(0));
}

void lumf() {
  bri = server.arg(0).toInt();
  att = LOW;
  server.send(200, "text/plain", "OK");
}

void huef() {
  hue = server.arg(0).toInt();
  att = LOW;
  server.send(200, "text/plain", "OK");
}

void satf() {
  sat = server.arg(0).toInt();
  att = LOW;
  server.send(200, "text/plain", "OK");
}


void onsf() {
  server.send(200, "text/plain", acceso ? "1" : "0");
}


void lumsf() {
  server.send(200, "text/plain", String(bri));
}

void huesf() {
  server.send(200, "text/plain", String(hue));
}

void satsf() {
  server.send(200, "text/plain", String(sat));
}

void setup() {
  analogWriteRange(255);
  pinMode(pinR, OUTPUT);
  digitalWrite(pinR, LOW);
  pinMode(pinG, OUTPUT);
  digitalWrite(pinG, LOW);
  pinMode(pinB, OUTPUT);
  digitalWrite(pinB, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
  server.on("/on", onf);
  server.on("/lum", lumf);
  server.on("/hue", huef);
  server.on("/sat", satf);
  server.on("/ons", onsf);
  server.on("/lums", lumsf);
  server.on("/sats", satsf);
  server.on("/hues", huesf);
  server.begin();
}

void loop() {
  server.handleClient();
  H2R_HSBtoRGB(hue, sat, bri*acceso, colore);
  if (colorev[0] < colore[0]) colorev[0]++;
  if (colorev[0] > colore[0]) colorev[0]--;
  if (colorev[1] < colore[1]) colorev[1]++;
  if (colorev[1] > colore[1]) colorev[1]--;
  if (colorev[2] < colore[2]) colorev[2]++;
  if (colorev[2] > colore[2]) colorev[2]--;
  analogWrite(pinR, colorev[0]);
  analogWrite(pinG, colorev[1]);
  analogWrite(pinB, colorev[2]);
  if (att && acceso) {
    hue++;
    if (hue == 361) hue = 0;
    delay(200);
  } else delay(5);
}
