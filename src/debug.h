/**
 * DIYSCIP (c) by Geoffroy HUBERT - yorffoeg@gmail.com
 * This file is part of DIYSCIP <https://github.com/yorffoeg/diyscip>.
 * 
 * DIYSCIP is licensed under a
 * Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 * 
 * You should have received a copy of the license along with this
 * work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.
 * 
 * DIYSCIP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;
 */ 

#pragma once
#include <Arduino.h>

#if ! defined(DBG_SERIAL_ENABLED) && ! defined(DBG_TCP_ENABLED)
#   define NODEBUG
#endif 

#if DBG_LED_ENABLED
#define DLED_INIT() { \
  pinMode(LED_BUILTIN, OUTPUT); \
  digitalWrite(LED_BUILTIN, HIGH); \
}
#define DLED_SET() { \
  digitalWrite(LED_BUILTIN, LOW); \
}
#define DLED_CLR() { \
  digitalWrite(LED_BUILTIN, HIGH); \
}
#else
#define DLED_INIT()
#define DLED_SET()
#define DLED_CLR()
#endif

#ifndef NODEBUG

    #ifdef DBG_TCP_ENABLED
        #include "ESP8266WiFi.h"

        extern WiFiClient clientDebug;

        #define DBG_TCP_CLIENT_PING     "%!PING#"
        #define DBG_TCP_CLIENT_PONG     "%!PONG#"

        #ifdef DBG_SERIAL_ENABLED

            #define DBG(str, ...)   { \
                Serial.printf(str, ##__VA_ARGS__); Serial.println("");  \
                clientDebug.printf(str, ##__VA_ARGS__); clientDebug.println(""); \
            }

            #define DBGNOLN(str, ...)   { \
                Serial.printf(str, ##__VA_ARGS__);  \
                clientDebug.printf(str, ##__VA_ARGS__); \
            }

        #else

            #define DBG(str, ...)   {  clientDebug.printf(str, ##__VA_ARGS__); clientDebug.println("");  }
            #define DBGNOLN(str, ...)   {  clientDebug.printf(str, ##__VA_ARGS__); }

        #endif // DBG_SERIAL_ENABLED

    #else

        #define DBG(str, ...)   { Serial.printf(str, ##__VA_ARGS__); Serial.println(""); }
        #define DBGNOLN(str, ...)   {  Serial.printf(str, ##__VA_ARGS__);   }

    #endif // DBG_TCP_ENABLED

#else 
    #define DBG(str, ...)
    #define DBGNOLN(str, ...)

#endif
