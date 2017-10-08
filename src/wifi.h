#ifndef _WIFIH_
#define _WIFIH_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "debug.h"

// variables needed for ESPWifi
const IPAddress STATIC_IP(192, 168, 1, 225);
const IPAddress GATEWAY_IP(192, 168, 1, 1);
const IPAddress SUBNET_IP(255, 255, 255, 0);
const char* WIFI_SSID = "Pretty Fly For a Wi-Fi";
const char* WIFI_PASS = "freckles";

void wifi_setup() {
  delay(5000);
  // We start by connecting to a WiFi network
  INFO_LOG("[WIFI] Connecting to %s\n", WIFI_SSID);
  // checks if already connected before WiFi.begin() to prevent loop
  WiFi.mode(WIFI_STA);
  WiFi.config(STATIC_IP, GATEWAY_IP, SUBNET_IP);
  if (WiFi.status() == WL_CONNECTED) {
    INFO_LOG("[WIFI] Already connected, exiting\n");
    return;
  }
  // attempts to connect to wifi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    INFO_LOG(".");
  }
  randomSeed(micros());
  INFO_LOG("[WIFI] Successfully connected\n");
}

#endif
