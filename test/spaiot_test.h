#pragma once
#include <Arduino.h>
//#define UNITY_INCLUDE_PRINT_FORMATTED
#include <unity.h>
#include <spaiotdebug.h>

#ifndef TEST_PRINTF
#ifndef DEBUG_ESP_PORT
#define DEBUG_ESP_PORT Serial
#endif
#define TEST_PRINTF(str, ...)   {  DEBUG_ESP_PORT.printf(str, ##__VA_ARGS__);   }
#endif
