#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#define rele 5        //D1
#define dindon 16     //D0
#define melodyPin 15  //D8
#define pincontatto 4 //D2
#define pinattivo 2   //D4

const char* ssid        = "XXXX";
const char* password    = "XXXX";
unsigned int stato = 3, lockTimeout = 1000, cont, cont0, suon = 0, sap = 1;
unsigned long unlockedTime = 0, tstart = 0;

ESP8266WebServer server(80);

void apri() {
  sap = server.arg(0).toInt();
  if (!sap){
    unlockedTime = millis();
    stato = 0;
  }
  server.send(200, "text/plain", server.arg(0));
}

void apris() {
  if (stato == 1 || stato == 2 ) server.send(200, "text/plain", "0");
  else server.send(200, "text/plain", String(sap));
}

void apertos() {
  if (stato <= 2) server.send(200, "text/plain", "0");
  else server.send(200, "text/plain", "1");
}

void attivo() {
  server.send(200, "text/plain", !digitalRead(pinattivo) ? "1" : "0");
}

void setup() {
  pinMode(rele, OUTPUT);
  digitalWrite(rele, LOW);
  pinMode(pincontatto, INPUT_PULLUP);
  pinMode(pinattivo, INPUT_PULLUP);
  pinMode(dindon, INPUT);
  tone(melodyPin, OUTPUT);
  noTone(melodyPin);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(300);
  }
  server.on("/apri", HTTP_POST, apri);
  server.on("/apris", HTTP_GET, apris);
  server.on("/apertos", HTTP_GET, apertos);
  server.on("/att", attivo);
  server.begin();
}

void loop() {
  server.handleClient();
  if (unlockedTime > 0 && (millis() - unlockedTime > lockTimeout)) {
    unlockedTime = 0;
    sap = 1;
    stato = 1;
  }
  digitalWrite(rele, !sap);
  if (!digitalRead(pincontatto)) stato = 2;
  if (stato == 2 && digitalRead(pincontatto)) stato = 3;
  if (digitalRead(dindon) && tstart == 0) tstart = millis();
  if (!digitalRead(dindon)) tstart = 0;
  if (digitalRead(dindon) && tstart > 0 && (millis() - tstart > 150)) {
    HTTPClient http;
    http.begin("http://XXXX:9053/ingresso");
    http.GET();
    http.end();
    cont = millis();
    if ((cont - cont0 >= 15000) || (suon > 1)) suon = 0;
    server.send(200, "text/plain", String(suon));
    if (!suon) ring0();
    else ring1();
    suon++;
    cont0 = cont;
    tstart = 0;
  }
}
