/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SpaIot library is licensed under a
   Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.

   You should have received a copy of the license along with this
   work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include <Arduino.h>
#include <map>

//! SpaIot name space
namespace SpaIot {

  /**
     @enum Key
     @brief LED and push buttons identification keys
  */
  enum Key {
    Power = 0,  //!< Power Led or Button
    Filter,     //!< Filter Led or Button
    Bubble,     //!< Bubble Led or Button
    Jet,        //!< Jet Led or Button
    Sanitizer,  //!< Sanitizer Led or Button
    Heater,     //!< Heater Led (Red) or Button
    HeatReached,//!< Heat Reached Led (Green)
    TempUp,     //!< Temperature Up Button
    TempDown,   //!< Temperature Down Button
    TempUnit,   //!< Temperature Unity Button (°C/°F)
  };

  /**
     @brief Association between keys and key names
  */
  const std::map<int, String> KeyName = {
    {Power, "Power"},
    {Filter, "Filter"},
    {Bubble, "Bubble"},
    {Jet, "Jet"},
    {Sanitizer, "Sanitizer"},
    {Heater, "Heater"},
    {HeatReached, "HeatReached"},
    {TempUp, "TempUp"},
    {TempDown, "TempDown"},
    {TempUnit, "TempUnit"}
  };

  /**
    @brief Association between keys and key names

    @param key   The key
    @return The key name, "Unknown" if the key is not found
  */
  inline const char *keyName (int key) {

    if (KeyName.find (key) != KeyName.end()) {

      return KeyName.at (key).c_str();
    }
    return "Unknown";
  }

  const uint16_t UnsetValue16 = 0xFFFF; //!< Indeterminate value on 16 bits
  const uint8_t UnsetValue8 = 0xFF; //!< Indeterminate value on 8 bits
  const unsigned long ButtonIntervalMs = 800; //!< Time between 2 successive actions on the buttons
  const uint16_t DesiredTempMax = 40; //!< Maximum desired temperature in °C
  const uint16_t DesiredTempMin = 20; //!< Minimum desired temperature in °C
  const uint16_t SetupTrigUnitChangeMin = 5; //!< Number of changes in temperature unit corresponding to the reset request
  const unsigned long SetupTrigUnitChangeStepMaxMs = 1000; //!< Maximum time separating successive actions on the unit change button to trigger reset request
  const unsigned long HoldPressedMs = 300; //!< Time between press and release a button
  const unsigned long BeginWaitingTimeMs = 3000; //!< \c begin() waiting time in milliseconds
  const unsigned long ResetErrorTimeMs = 4000; //!< Time after which an error can be erased
}
