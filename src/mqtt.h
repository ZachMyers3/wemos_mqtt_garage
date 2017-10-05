#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include "debug.h"

// setup for MQTT communication
WiFiClient espClient;
PubSubClient client(espClient);

const char* MQTT_SERVER = "m13.cloudmqtt.com";
const int MQTT_PORT = 12882;
const char* MQTT_USERNAME = "ldltvixx";
const char* MQTT_PASSWORD = "ttdOEBRDHhd1";

void mqtt_connect() {
  DEBUG_LOG("[MQTT] Connecting to %s\n", MQTT_SERVER);
  if (client.connect("ESP8266Client", MQTT_USERNAME, MQTT_PASSWORD)) {
    DEBUG_LOG("[MQTT] Connected\n");
  } else {
    DEBUG_LOG("[MQTT] Connect failed rc=%s, trying again\n", client.state());
    delay(2000);
  }
}
