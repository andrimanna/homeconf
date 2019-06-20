#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define pinrele 5   // Pin D1 

const char* ssid        = "XXXX";
const char* password    = "XXXX";

unsigned int currentmillis = 0, previusmillis = 0, dbs = 0;
boolean acceso = LOW, notime = LOW;
int vel=100;

ESP8266WebServer server(80);

void onf() {
  if (server.arg(0) == "true"){
    acceso = HIGH;
    notime = HIGH;
    if (vel > 25) dbs = 1;
    else dbs = 2;
  }
  else {
    acceso = LOW;
    dbs = 0;
  }
  server.send(200, "text/plain", server.arg(0));
}

void db() {
  server.send(200, "text/plain", "OK");
  notime = LOW;
  dbs++;
  if (dbs > 2) dbs = 0;
}

void onsf() {
  server.send(200, "text/plain", acceso ? "1" : "0");
}

void velf() {
  vel = server.arg(0).toInt();
  server.send(200, "text/plain", "OK");
}

void velsf() {
  server.send(200, "text/plain", String(vel));
}

void setup() {
  analogWriteRange(100);
  pinMode(pinrele, OUTPUT);
  digitalWrite(pinrele, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
  server.on("/on", onf);
  server.on("/ons", onsf);
  server.on("/vel", velf);
  server.on("/vels", velsf);
  server.on("/db", db);
  server.begin();
}

void loop() {
  server.handleClient();
  if (!notime){
    if (dbs == 0) acceso = LOW;
    else if (dbs == 1) {
      acceso = HIGH;
      vel = 100;
    } else if (dbs == 2){
      acceso = HIGH;
      vel = 25;
    }
  }
  if (acceso) {
    currentmillis = millis();
    if (currentmillis - previusmillis >=  3.6e6) {
      previusmillis = currentmillis;
      acceso = LOW;
      dbs = 0;
    }
  } else previusmillis = millis();
  analogWrite(pinrele, acceso*vel);
}
