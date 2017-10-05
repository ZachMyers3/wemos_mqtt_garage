/*
* wemos_mqtt_template
*
* This program acts as a working example of sending / recieving mqtt
* data to and from an mqtt server. Listed are the steps needed to test
* on a local machine.
*/

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include "debug.h"
#include "ota.h"
#include "wifi.h"
#include "mqtt.h"

bool blink = false;
bool blink_switch = true;

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String strTopic = topic;
  String strMsg = "";
  for (int i = 0; i < length; i++) {
    strMsg += (char)payload[i];
  }
  DEBUG_LOG("[MQTT|%s] %s\n", strTopic.c_str(), strMsg.c_str());
  if (strTopic == "esp/test") {
    DEBUG_LOG("[MQTT] Valid topic located\n");
    if (strMsg == "blink") {
      DEBUG_LOG("[MQTT] Toggling blink switch\n");
      blink_switch = !blink_switch;
      client.publish("esp/test", "Blink switch toggled\n");
    }
  }
}

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  DEBUG_LOG("[ESP]  Starting setup\n");
  wifi_setup();
  ota_setup();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqtt_callback);
  while (!client.connected()) { mqtt_connect(); }
  client.publish("esp/test", "Hello from ESP8266");
  client.subscribe("esp/test");
  // initialize LED digital pin as an output
  pinMode(LED_BUILTIN, OUTPUT);
  DEBUG_LOG("[WIFI] IP: %s\n", WiFi.localIP().toString().c_str());
  DEBUG_LOG("[ESP]  Setup finished");
}

void loop() {
  // handle OTA updates in loop
  ArduinoOTA.handle();
  // remain connected to mqtt service
  if (!client.connected()) { mqtt_connect(); }
  // blink LED
  if (blink_switch) {
    if (blink) { digitalWrite(LED_BUILTIN, HIGH); }
    else { digitalWrite(LED_BUILTIN, LOW); }
    blink = !blink;
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  // continue loop for mqtt data
  client.loop();
  // built in delay
  delay(2000);
}
