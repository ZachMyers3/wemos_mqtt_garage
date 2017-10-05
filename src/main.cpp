/*
* wemos_mqtt_template
*
* This program acts as a working example of sending / recieving mqtt
* data to and from an mqtt server. Listed are the steps needed to test
* on a local machine.
*/

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

// variables needed for OTA / ESPWifi
const char* WIFI_SSID = "Pretty Fly For a Wi-Fi";
const char* WIFI_PASS = "freckles";
//const char* mqtt_server = "zach-ardt";
// development IP address
// const char* mqtt_server = "192.168.1.175";
const char* MQTT_SERVER = "m13.cloudmqtt.com";
const int MQTT_PORT = 12882;
const char* MQTT_USERNAME = "ldltvixx";
const char* MQTT_PASSWORD = "ttdOEBRDHhd1";

bool blink = false;
bool blink_switch = true;

// setup for MQTT communication
WiFiClient espClient;
PubSubClient client(espClient);

void mqtt_connect() {
  Serial.printf("[MQTT] Connecting to %s\n", MQTT_SERVER);
  if (client.connect("ESP8266Client", MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.println("[MQTT] Connected");
  } else {
    Serial.printf("[MQTT] Connect failed rc=%s, trying again\n", client.state());
    delay(2000);
  }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  //Serial.print("[MQTT|");
  //Serial.print(topic);
  //Serial.print("]: ");
  String strTopic = topic;
  String strMsg = "";
  for (int i = 0; i < length; i++) {
    strMsg += (char)payload[i];
  }
  Serial.printf("[MQTT|%s] %s\n", strTopic.c_str(), strMsg.c_str());
  if (strTopic == "esp/test") {
    Serial.println("[MQTT] Valid topic located");
    if (strMsg == "blink") {
      Serial.println("[MQTT] Toggling blink switch");
      blink_switch = !blink_switch;
    }
  }
}

void mqtt_setup() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqtt_callback);
  while (!client.connected()) { mqtt_connect(); }
}

void wifi_setup() {
  delay(5000);
  // We start by connecting to a WiFi network
  printf("[WIFI] Connecting to %s\n", WIFI_SSID);
  // checks if already connected before WiFi.begin() to prevent loop
  WiFi.mode(WIFI_STA);
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[WIFI] Already connected, exiting");
    return;
  }
  // attempts to connect to wifi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("[WIFI] Successfully connected");
}

void ota_setup() {
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("[OTA]  Started");
}

void setup() {
  Serial.begin(115200);
  Serial.println("[ESP]  Starting setup");
  wifi_setup();
  ota_setup();
  mqtt_setup();
  client.publish("esp/test", "Hello from ESP8266");
  client.subscribe("esp/test");
  // initialize LED digital pin as an output
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.print("[WIFI] IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("[ESP]  Setup finished");
}

void loop() {
  // handle OTA updates in loop
  ArduinoOTA.handle();
  // remain connected to wifi
  if (!client.connected()) {
    mqtt_connect();
  }
  // blink LED
  if (blink_switch) {
    if (blink) { digitalWrite(LED_BUILTIN, HIGH); }
    else { digitalWrite(LED_BUILTIN, LOW); }
    blink = !blink;
  }
  // continue loop for mqtt data
  client.loop();
  // built in delay
  delay(2000);
}
