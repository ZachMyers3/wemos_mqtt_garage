#ifndef _DEBUGH_
#define _DEBUGH_

#include <stdio.h>
#include <stdarg.h>
#include <Arduino.h>

// toggle switch for debugging
#define DEBUG 1

// debug function message
void DEBUG_LOG(const char *format, ...)
{
#ifdef DEBUG
    char buff[1024] = {'\0'};
    va_list arguments;
    va_start(arguments, format);
    vsnprintf(buff, sizeof(buff), format, arguments);
    Serial.print(buff);
    va_end(arguments);
#endif
}

#endif
