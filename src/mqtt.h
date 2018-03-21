#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include "debug.h"

// setup for MQTT communication
WiFiClient espClient;
PubSubClient client(espClient);

const char* MQTT_SERVER = "";
const int MQTT_PORT = 0;
const char* MQTT_USERNAME = "";
const char* MQTT_PASSWORD = "";

void mqtt_connect() {
  espClient = new WifiClient();
  client.setClient(*espClient);
  INFO_LOG("[MQTT] Connecting to %s\n", MQTT_SERVER);
  if (client.connect("ESP8266Client", MQTT_USERNAME, MQTT_PASSWORD)) {
    INFO_LOG("[MQTT] Connected\n");
  } else {
    ERROR_LOG("[MQTT] Connect failed rc=%s, trying again\n", client.state());
    delay(2000);
  }
  delete(espClient);
}
