#ifndef _WIFIH_
#define _WIFIH_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "debug.h"

// variables needed for ESPWifi
const char* WIFI_SSID = "Pretty Fly For a Wi-Fi";
const char* WIFI_PASS = "freckles";

void wifi_setup() {
  delay(5000);
  // We start by connecting to a WiFi network
  DEBUG_LOG("[WIFI] Connecting to %s\n", WIFI_SSID);
  // checks if already connected before WiFi.begin() to prevent loop
  WiFi.mode(WIFI_STA);
  if (WiFi.status() == WL_CONNECTED) {
    DEBUG_LOG("[WIFI] Already connected, exiting\n");
    return;
  }
  // attempts to connect to wifi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DEBUG_LOG(".");
  }
  randomSeed(micros());
  DEBUG_LOG("[WIFI] Successfully connected\n");
}

#endif
