/*
 * SpaIot Library (c) by espilonrt - epsilonrt@gmail.com
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

#if ! defined(NODEBUG_SPAIOT) && defined(DEBUG_ESP_PORT)
  #define DBG(str, ...)   { DEBUG_ESP_PORT.printf(str, ##__VA_ARGS__); DEBUG_ESP_PORT.println(""); }
  #define DBGNOLN(str, ...)   {  DEBUG_ESP_PORT.printf(str, ##__VA_ARGS__);   }

#else 
    #define DBG(str, ...)
    #define DBGNOLN(str, ...)
#endif

#if ! defined(NODEBUG_SPAIOT) && defined(DEBUG_LED)
  #ifndef DEBUG_LED_ONSTATE
    #define DLED_LOW HIGH
    #define DLED_HIGH LOW
  #else
    #if DEBUG_LED_ONSTATE == HIGH
      #define DLED_LOW LOW
      #define DLED_HIGH HIGH
    #else
      #define DLED_LOW HIGH
      #define DLED_HIGH LOW
    #endif
  #endif
  
  #define DLED_INIT() { \
    pinMode(DEBUG_LED, OUTPUT); \
    digitalWrite(DEBUG_LED, DLED_LOW); \
  }
  #define DLED_SET() { \
    digitalWrite(DEBUG_LED, DLED_HIGH); \
  }
  #define DLED_CLR() { \
    digitalWrite(DEBUG_LED, DLED_LOW); \
  }

#else
#define DLED_INIT()
#define DLED_SET()
#define DLED_CLR()
#endif
