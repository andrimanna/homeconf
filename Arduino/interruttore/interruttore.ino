#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SimpleDHT.h>

#define releila 5   // Pin D1 
#define releint 4   // Pin D2
#define pindht22 2  // Pin D4
#define dist 0.60

SimpleDHT22 dht22(pindht22);

const char* ssid        = "XXXX";
const char* password    = "XXXX";
unsigned int tempo, tempo0;
float tcorr = 25.0, ttarg = 25.0, ucorr = 50.0, temp;
int scorr, starg, acceso = 0, valore = 0;

ESP8266WebServer server(80);

void stacor() {
  server.send(200, "text/plain", String(scorr));
}

void staobi() {
  starg = server.arg(0).toInt();
  server.send(200, "text/plain", String(starg));
}

void staobis() {
  server.send(200, "text/plain", String(starg));
}

void temcor() {
  char rsp[255];
  sprintf(rsp, "{\"temperatura\": %.2f}", tcorr);
  server.send(200, "text/plain", rsp);
}

void temobi() {
  temp = server.arg(0).toFloat();
  if (temp) ttarg = temp;
  server.send(200, "text/plain", String(ttarg));
}

void temobis() {
  server.send(200, "text/plain", String(ttarg));
}

void nulla() {
  server.send(200, "text/plain", "0");
}
void umiatt() {
  char rsp[255];
  sprintf(rsp, "{\"umidita\": %.2f}", ucorr);
  server.send(200, "text/plain", rsp);
}

void interr() {
  if (server.arg(0) == "true") acceso = 1;
  else acceso = 0;
  HTTPClient http;
  http.begin("http://XXXX/att");
  int httpCode = http.GET();
  if (httpCode > 0) {
    valore = http.getString().toInt();
    if (valore != acceso) { 
      digitalWrite(releint, LOW);
      server.send(200, "text/plain", "ok");
      delay(1000);
      digitalWrite(releint, HIGH);
    } else server.send(200, "text/plain", "già in quello stato");
  } else server.send(200, "text/plain", "errore");
  http.end();
}

void setup() {
  pinMode(releila, OUTPUT);
  digitalWrite(releila, HIGH);
  pinMode(releint, OUTPUT);
  digitalWrite(releint, HIGH);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
  server.on("/sc", stacor);
  server.on("/so", staobi);
  server.on("/sos", staobis);
  server.on("/tc", temcor);
  server.on("/to", temobi);
  server.on("/tos", temobis);
  server.on("/", nulla);
  server.on("/ua", umiatt);
  server.on("/on", interr);
  server.begin();
}

void loop() {
  server.handleClient();
  tempo = millis();
  if (tempo - tempo0 >= 10000) {
    tempo0 = tempo;
    float tempt = 0;
    float humt = 0;
    if (SimpleDHTErrSuccess == dht22.read2(&tempt, &humt, NULL)) {
      tcorr = tempt;
      ucorr = humt;
    }
    float delta = tcorr - ttarg;
    switch (starg) {
      case 1:
        if (delta <= -dist) scorr = 1;
        else if (delta >= -dist && delta <= 0 && scorr == 1) scorr = 1;
        else scorr = 0;
        break;
      case 2:
        if (delta >= dist) scorr = 2;
        else if (delta >= 0 && delta <= dist && scorr == 2) scorr = 2;
        else scorr = 0;
        break;
      default:
        scorr = 0;
    }
    if (scorr) digitalWrite(releila, LOW);
    else digitalWrite(releila, HIGH);
  }
}
