#ifndef _OTAH_
#define _OTAH_

#include <Arduino.h>
#include <ArduinoOTA.h>
#include "debug.h"

void ota_setup() {
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    DEBUG_LOG("Start updating %s\n", type.c_str());
  });
  ArduinoOTA.onEnd([]() {
    DEBUG_LOG("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    DEBUG_LOG("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    DEBUG_LOG("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) DEBUG_LOG("Auth Failed\n");
    else if (error == OTA_BEGIN_ERROR) DEBUG_LOG("Begin Failed\n");
    else if (error == OTA_CONNECT_ERROR) DEBUG_LOG("Connect Failed\n");
    else if (error == OTA_RECEIVE_ERROR) DEBUG_LOG("Receive Failed\n");
    else if (error == OTA_END_ERROR) DEBUG_LOG("End Failed\n");
  });
  ArduinoOTA.begin();
  DEBUG_LOG("[OTA]  Started\n");
}

#endif
