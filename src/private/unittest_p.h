/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#ifdef PIO_UNIT_TESTING
#include <unity.h>
// #warning "Compiling for unit testing"
#else
#ifdef DEBUG_SERIAL_PORT
#define TEST_PRINTF(fmt,...)  { DEBUG_SERIAL_PORT.printf_P(PSTR(fmt),##__VA_ARGS__); DEBUG_SERIAL_PORT.println(""); }
#define TEST_MESSAGE(message)  { DEBUG_SERIAL_PORT.println(F(message)); }
#else
#define TEST_PRINTF(fmt,...)
#define TEST_MESSAGE(message)
#endif
#endif
