#ifndef _DEBUGH_
#define _DEBUGH_

#include <stdio.h>
#include <stdarg.h>
#include <Arduino.h>
#include <RemoteDebug.h>

RemoteDebug RSerial;

// toggle switch for debugging
#define DEBUGGING 1

void VERBOSE_LOG(const char *format, ...)
{
#ifdef DEBUGGING
    char buff[1024] = {'\0'};
    va_list arguments;
    va_start(arguments, format);
    vsnprintf(buff, sizeof(buff), format, arguments);
    if (RSerial.isActive(RSerial.VERBOSE)) {
      RSerial.print(buff);
    }
    va_end(arguments);
#endif
}

void DEBUG_LOG(const char *format, ...)
{
#ifdef DEBUGGING
    char buff[1024] = {'\0'};
    va_list arguments;
    va_start(arguments, format);
    vsnprintf(buff, sizeof(buff), format, arguments);
    if (RSerial.isActive(RSerial.DEBUG)) {
      RSerial.print(buff);
    }
    va_end(arguments);
#endif
}

void INFO_LOG(const char *format, ...)
{
#ifdef DEBUGGING
    char buff[1024] = {'\0'};
    va_list arguments;
    va_start(arguments, format);
    vsnprintf(buff, sizeof(buff), format, arguments);
    if (RSerial.isActive(RSerial.INFO)) {
      RSerial.print(buff);
    }
    va_end(arguments);
#endif
}

void WARNING_LOG(const char *format, ...)
{
#ifdef DEBUGGING
    char buff[1024] = {'\0'};
    va_list arguments;
    va_start(arguments, format);
    vsnprintf(buff, sizeof(buff), format, arguments);
    if (RSerial.isActive(RSerial.WARNING)) {
      RSerial.print(buff);
    }
    va_end(arguments);
#endif
}

void ERROR_LOG(const char *format, ...)
{
#ifdef DEBUGGING
    char buff[1024] = {'\0'};
    va_list arguments;
    va_start(arguments, format);
    vsnprintf(buff, sizeof(buff), format, arguments);
    if (RSerial.isActive(RSerial.ERROR)) {
      RSerial.print(buff);
    }
    va_end(arguments);
#endif
}

#endif
