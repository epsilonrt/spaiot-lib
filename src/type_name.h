/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include <Arduino.h>

/**
    @brief A template function that returns the type name of the parameter.

    typeid() is not used because it is not supported by all platforms (with -fno-rtti enabled).
    for example, the ESP32 compiler does not support it:
    > error: cannot use 'typeid' with -fno-rtti

*/
template <class T>
String type_name (const T &) {
  String s = __PRETTY_FUNCTION__;
  int start = s.indexOf ("[with T = ") + 10;
  int stop = s.lastIndexOf (']');
  return s.substring (start, stop);
}
