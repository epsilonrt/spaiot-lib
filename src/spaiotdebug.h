/*
 * SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
 * This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>
 *
 * SpaIot library is licensed under a
 * Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * You should have received a copy of the license along with this
 * work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.
 *
 * SpaIot library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;
 */

#pragma once

#include <Arduino.h>
#include "private/unittest_p.h"

#if ! defined(NODEBUG_SPAIOT) && defined(DEBUG_ESP_PORT)
  #define SPAIOT_DBG(...)   { DEBUG_ESP_PORT.printf(__VA_ARGS__); DEBUG_ESP_PORT.println(""); }
  #define SPAIOT_DBGP(...)   { DEBUG_ESP_PORT.printf_P(__VA_ARGS__); DEBUG_ESP_PORT.println(""); }
  #define SPAIOT_DBGNOLN(...)   {  DEBUG_ESP_PORT.printf(__VA_ARGS__);   }
  #define SPAIOT_DBGNOLNP(...)   {  DEBUG_ESP_PORT.printf_P(__VA_ARGS__);   }
  #define SPAIOT_ASSERT(cond, str, ...) { if (!(cond)) SPAIOT_DBG(str, ##__VA_ARGS__); }

#else 
  #define SPAIOT_DBG(...)
  #define SPAIOT_DBGP(...)
  #define SPAIOT_DBGNOLN(...)
  #define SPAIOT_DBGNOLNP(...)
  #define SPAIOT_ASSERT(cond, str, ...)
#endif

#if ! defined(NODEBUG_SPAIOT) && defined(DEBUG_LED)
  #ifndef DEBUG_LED_ONSTATE
    #define SPAIOT_DBGLED_LOW HIGH
    #define SPAIOT_DBGLED_HIGH LOW
  #else
    #if DEBUG_LED_ONSTATE == HIGH
      #define SPAIOT_DBGLED_LOW LOW
      #define SPAIOT_DBGLED_HIGH HIGH
    #else
      #define SPAIOT_DBGLED_LOW HIGH
      #define SPAIOT_DBGLED_HIGH LOW
    #endif
  #endif
  
  #define SPAIOT_DBGLED_INIT() { \
    pinMode(DEBUG_LED, OUTPUT); \
    digitalWrite(DEBUG_LED, SPAIOT_DBGLED_LOW); \
  }
  #define SPAIOT_DBGLED_SET() { \
    digitalWrite(DEBUG_LED, SPAIOT_DBGLED_HIGH); \
  }
  #define SPAIOT_DBGLED_CLR() { \
    digitalWrite(DEBUG_LED, SPAIOT_DBGLED_LOW); \
  }

#else
#define SPAIOT_DBGLED_INIT()
#define SPAIOT_DBGLED_SET()
#define SPAIOT_DBGLED_CLR()
#endif
