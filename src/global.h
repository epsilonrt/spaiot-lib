/**
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

#include <cstdint>

namespace SpaIot {

  enum  {
    Power = 0,
    Filter,
    Bubble,
    Jet,
    Sanitizer,
    Heater,
    HeatReached, // led only
    TempUp, // button only
    TempDown, // button only
    TempUnit, // button only
  };

  const uint16_t UnsetValue16 = 0xFFFF;
  const uint8_t UnsetValue8 = 0xFF;
  const unsigned long ButtonIntervalMs = 500;
  const uint16_t DesiredTempMax = 40;
  const uint16_t DesiredTempMin = 20;
  const uint16_t SetupTrigUnitChangeMin = 5;
  const unsigned long SetupTrigUnitChangeStepMaxMs = 1000;
  const unsigned long HoldPressedMs = 300;
}
