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

/**
 *  A template function that returns the type name of the parameter. 
 */
template <class T>
String type_name (const T&) {
  String s = __PRETTY_FUNCTION__;
  int start = s.indexOf ("[with T = ") + 10;
  int stop = s.lastIndexOf (']');
  return s.substring (start, stop);
}
