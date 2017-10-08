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
#include <RemoteDebug.h>
// in-house code moved to separate files for cleanliness
#include "debug.h"
#include "ota.h"
#include "wifi.h"
#include "mqtt.h"

const char* HOSTNAME = "ESP_Test";
const char* MQTT_GARAGE_TOPIC = "esp/garage";
const char* MQTT_GARAGE_TOGGLE = "TOGGLE";

bool blink = false;
bool blink_switch = true;

// garage control variables
const int RELAY_PIN = D1;

// proximity sensor variables
const int DOOR_PIN = D2;
String door_state = "UNDEFINED";
String last_state, str_topic, str_msg;
long last_mqtt_msg;

/*
  Callback message handler for incoming MQTT messages. Assigns variables
  str_topic and str_msg to their variables from the incoming messages.
*/
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  str_topic = topic;
  str_msg = "";
  for (int i = 0; i < length; i++) {
    str_msg += (char)payload[i];
  }
  DEBUG_LOG("[MQTT|%s] %s\n", str_topic.c_str(), str_msg.c_str());
  if (str_topic == "esp/test") {
    DEBUG_LOG("[MQTT] Valid topic located\n");
    if (str_msg == "blink") {
      DEBUG_LOG("[MQTT] Toggling blink switch\n");
      blink_switch = !blink_switch;
      client.publish("esp/test", "Blink switch toggled\n");
    }
  }
}

/*
  Sets door_state to either open or closed based on the status of the door pin
  monitor
*/
void garage_status() {
  last_state = door_state;
  if (digitalRead(DOOR_PIN)) {
    door_state = "CLOSED";
  } else if (!(digitalRead(DOOR_PIN))) {
    door_state = "OPEN";
  }

  if (last_state != door_state) {
    client.publish(MQTT_GARAGE_TOPIC, door_state.c_str());
    DEBUG_LOG("[MQTT|%s] %s", MQTT_GARAGE_TOPIC, door_state.c_str());
  }

  long now = millis();
  if (now - last_mqtt_msg > 60000) {
    last_mqtt_msg = now;
    client.publish(MQTT_GARAGE_TOPIC, door_state.c_str());
    DEBUG_LOG("[MQTT|%s] %s", MQTT_GARAGE_TOPIC, door_state.c_str());
  }
}

/*
  Checks for a valid MQTT message to toggle the garage door using the relay,
  exits otherwise
*/
void garage_status_handler() {
  // guards for invalid topic and message
  if (str_topic != MQTT_GARAGE_TOPIC) { return; }
  if (str_msg != MQTT_GARAGE_TOGGLE) { return; }

  // valid message, toggle garage
  DEBUG_LOG("[ESP]  Toggling garage controller relay\n");
  // pinMode(RELAY_PIN, HIGH);
  // delay(600);
  // pinMode(RELAY_PIN, LOW);
  // clear topic and message to avoid overlap
  str_topic, str_msg = "";
}

void setup() {
#ifdef DEBUGGING
  Serial.begin(115200);
  RSerial.begin(HOSTNAME);
  RSerial.setSerialEnabled(true);
  RSerial.handle();
#endif
  DEBUG_LOG("[ESP]  Starting setup\n");
  wifi_setup();
  DEBUG_LOG("[WIFI] IP: %s\n", WiFi.localIP().toString().c_str());
  ota_setup();

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqtt_callback);
  while (!client.connected()) { mqtt_connect(); }
  client.subscribe(MQTT_GARAGE_TOPIC);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN, LOW);
  pinMode(DOOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RELAY_PIN, LOW);

  DEBUG_LOG("[ESP]  Setup finished\n");
}

void loop() {
#ifdef DEBUG
  // RSerial.printf("PLS PLS PLS\n");
  RSerial.handle();
#endif
  ArduinoOTA.handle();
  if (!client.connected()) { mqtt_connect(); }

  // garage_status();
  garage_status_handler();

  client.loop();
  delay(2000);
}
