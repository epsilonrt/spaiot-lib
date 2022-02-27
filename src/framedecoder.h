/*
 * SpaIot Library (c) by espilonrt - epsilonrt@gmail.com
 * This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>
 * This file is based on DIYSCIP (c) by Geoffroy HUBERT
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

#include <map>
#include "bussettings.h"

namespace SpaIot {

  /**
   * @class FrameDecoder
   * @brief This class decodes information from the technical block of the spa
   *
   * This class decodes the frames so as to retrieve the condition of the LEDs 
   * and the value present on the 7-segment display.It can not detect if the user 
   * presses a button.\n
   * FrameDecoder stores the bits of the frame on each interrupt triggered by 
   * the clock rising edges, once the 16 bits of the raw status received, it 
   * decodes this status on the rising edge of the nWR (HOLD) signal.
   *
   * Base class that can not be instantiated !
   */
  class FrameDecoder {
  public:
      /**
       * @brief Initializes and connect with the spa
       * 
       * This function must be called before you can use the instance.
       * This function configures the different hardware features, then loop 
       * waiting for a frame for a time of \c BeginWaitingTimeMs milliseconds.
       * \c isOpened() lets you know if the connection has been successfully completed
       */
      void begin();
      /**
       * @brief Indicates whether the connection with the spa is established.
       * 
       * @return If \c begin() succeeded and everything works returns true.
       * Returns false if the connection to the spa is not established and no frame is received.
       */
      bool isOpened() const;
      /**
       * @brief Check if the hardware configuration has the LED
       * @param key LED identification key in the possible values of SpaIot::Key
       * @return true if the LED exists
       */
      bool hasLed (int key) const;
      /**
       * @brief Bus settings provides at the instantiation
       * @return constant reference on settings
       */
      const BusSettings & busSettings() const;
      /**
       * @brief Leds settings provides at the instantiation
       * @return Constant reference on the [std::map](https://en.cppreference.com/w/cpp/container/map)  containing the LED settings. 
       * The different key values are defined by SpaIot::Key
       */
      const std::map <int, LedSettings> ledSettings() const;
      /**
       * @brief Last state of the LEDs received
       * 
       * Pour la mise au point uniquement. 
       * @return The value corresponding to the last control frame of the LEDs, 
       * the value is the binary complement of the frame signal. 
       * UnsetValue16 is returned if no frame has been received.
       */
      uint16_t  rawStatus() const;
      /**
       * @brief Last state received from an LED
       * @param key LED identification key in the possible values of SpaIot::Key
       * @return true if the LED is lit, false if it is off, UnsetValue8 if this 
       * LED does not exist or if no frame has been received.
       */
      uint8_t   isLedOn (int key) const;
      /**
       * @brief Last state of the Power LED
       * @return true if the LED is lit, false if it is off, UnsetValue8 if this 
       * LED does not exist or if no frame has been received.
       */
      inline uint8_t   isPowerOn() const;
      /**
       * @brief Last state of the Filter LED
       * 
       * Lets you know if the water filtration pump is started.
       * @return true if the LED is lit, false if it is off, UnsetValue8 if this 
       * LED does not exist or if no frame has been received.
       */
      inline uint8_t   isFilterOn() const;
      /**
       * @brief Last state of the Bubble LED
       * 
       * Lets you know if the bubble generator is started.
       * @return true if the LED is lit, false if it is off, UnsetValue8 if this 
       * LED does not exist or if no frame has been received.
       */
      inline uint8_t   isBubbleOn() const;
      /**
       * @brief Last state of the HeatReached LED
       * 
       * Lets you know if the water temperature is greater than or equal to the 
       * desired temperature.
       * @return true if the LED is lit, false if it is off, UnsetValue8 if this 
       * LED does not exist or if no frame has been received.
       */
      inline uint8_t   isHeatReached() const;
      /**
       * @brief Last state of the Jet LED
       * 
       * Lets you know if the water jets are activated.
       * @return true if the LED is lit, false if it is off, UnsetValue8 if this 
       * LED does not exist or if no frame has been received.
       */
      inline uint8_t   isJetOn() const;
      /**
       * @brief Last state of the Sanitizer LED
       * 
       * Lets you know if the sanitation of the water is in progress.
       * @return true if the LED is lit, false if it is off, UnsetValue8 if this 
       * LED does not exist or if no frame has been received.
       */
      inline uint8_t   isSanitizerOn() const;
      /**
       * @brief State of water heating
       * 
       * Indicates that the heating is started, that the temperature of the 
       * water is reached or not
       * 
       * @return true if the heater is started, false if it is off, UnsetValue8 
       * if no frame has been received.
       */
      uint8_t   isHeaterOn() const;
      /**
       * @brief Water temperature in °C
       * 
       * Framedecoder must wait to be sure that the display does not blink and 
       * the value is stable before determining the temperature of the water.\n  
       * It can take 20 seconds. waitForWaterTemp() can be used to wait for 
       * FrameDecoder to determine the temperature.
       * @return  Water temperature in °C, UnsetValue16 if it has not been determined yet.
       */
      uint16_t  waterTemp() const;
      /**
       * @brief Water temperature desired in °C
       * 
       * The desired temperature can only be known if the user has made a setting. 
       * Indeed, this temperature is only displayed when the user presses the 
       * SpaIot::TempUp or SpaIot::TempDown buttons (blinking the display).\n 
       * We can use ControlPanel::waitForDesiredTemp() to perform this operation 
       * and can read a correct value.
       * @return  Water temperature desired in °C, UnsetValue16 if it has not 
       * been determined yet.
       */
      uint16_t  desiredTemp() const;
      /**
       * @brief Remaining sanitation time
       * @return Remaining sanitation time in hours, UnsetValue16 if it has not 
       * been determined yet.
       */
      uint16_t  sanitizerTime() const;
      /**
       * @brief Number of frames received from startup
       * 
       * @return Number of frames received including the frames dropped
       */
      uint32_t  frameCounter() const;
      /**
       * @brief Number of dropped frames
       * 
       * If a new frame is received before decoding the previous frame is 
       * complete, this frame is lost. If the value of this counter increases 
       * rapidly is that the number of operations running under interruption by 
       * the CPU is important (the real time constraint is too high).\n 
       * It is important to adjust the frequency of the CPU to its maximum 
       * value (160MHz for ESP8266).
       * @return Number of frames dropped
       */
      uint32_t  frameDropped() const;
      /**
       * @brief Error code displayed by the control panel
       * 
       * For example, the E90 value can be displayed to indicate a water 
       * circulation problem. The user manual must be consulted for the meaning 
       * of this code.\n
       * This value is reset when calling this function if no error code has 
       * been displayed for more than 4 seconds (SpaIot::ResetErrorTimeMs).
       * @return The error code, 0 in normal situation
       */
      uint16_t  error();
      /**
       * @brief Reset request triggered
       *
       * The user can trigger a reset of settings by quickly changing 
       * SpaIot::SetupTrigUnitChangeMin times the temperature unit.
       * @sa SpaIot::SetupTrigUnitChangeMin
       * @sa SpaIot::SetupTrigUnitChangeStepMaxMs
       * @return true if triggered
       */
      bool isSetupModeTriggered() const;
      /**
       * @brief Check if the display blink
       * @return true if blinking, false if not, UnsetValue8 
       * if no frame has been received.
       */
      uint8_t isDisplayBlink() const;
      /**
       * @brief Wait until the display blink
       * @param MaxWaitingTimeMs Maximum waiting time in milliseconds
       * @return true if blinking, false if not, UnsetValue8 
       * if no frame has been received.
       */
      uint8_t waitUntilDisplayBlink (unsigned long MaxWaitingTimeMs = 5000) const;
      /**
       * @brief Wait until the water temperature could be read
       * 
       * Framedecoder must wait to be sure that the display does not blink and 
       * the value is stable before determining the temperature of the water.\n  
       * It can take 20 seconds. waitForWaterTemp() can be used to wait for 
       * FrameDecoder to determine the temperature.
       * @param MaxWaitingTimeMs Maximum waiting time in milliseconds
       * @return  Water temperature in °C, UnsetValue16 if it has not been determined yet.
       */
      uint16_t waitForWaterTemp (unsigned long MaxWaitingTimeMs = 25000) const;

    protected:
      FrameDecoder (const BusSettings & bus,
                  const std::map <int, LedSettings> & leds);
      static uint16_t  convertDisplayToCelsius (uint16_t m_displayValue);

    protected:
      const BusSettings & m_busSettings;
      const std::map <int, LedSettings> & m_ledSettings;

      // Parameters set by the constructor
      static int  m_dataPin;
      static uint16_t m_frameLedPower;
      static uint16_t m_frameLedFilter;
      static uint16_t m_frameLedHeater;
      static uint16_t m_frameLedHeaterReached;
      static uint16_t m_frameLedBubble;
      static uint16_t m_frameLedJet;
      static uint16_t m_frameLedSanitizer;

      // Data members of Decoder, updated by interrupt service routines
      static volatile uint32_t  m_frameCounter;
      static volatile uint32_t  m_frameDropped;
      static volatile uint16_t  m_rawStatus;
      static volatile uint16_t  m_errorValue;
      static volatile uint16_t  m_waterTemp;
      static volatile uint16_t  m_desiredTemp;
      static volatile uint16_t  m_sanitizerTime;

      // Working status variables, updated by interrupt service routines
      static volatile uint16_t  m_frameValue;
      static volatile uint16_t  m_frameShift;
      static volatile uint16_t  m_displayValue;
      static volatile uint32_t  m_lastSanitizerFrameCounter;
      static volatile uint32_t  m_lastBlackDisplayFrameCounter;
      static volatile bool      m_isDisplayBlink;
      static volatile uint32_t  m_lastErrorChangeFrameCounter;
      static volatile uint16_t  m_latestLedStatus;
      static volatile uint16_t  m_stableLedStatusCounter;
      static volatile uint16_t  m_latestDisplayValue;
      static volatile uint16_t  m_stableDisplayValueCounter;
      static volatile uint16_t  m_latestDesiredTemp;
      static volatile uint16_t  m_latestWaterTemp;
      static volatile uint16_t  m_stableWaterTempCounter;
      static volatile uint16_t  m_unsetDigits;
      static volatile uint8_t   m_lastTempUnit;
      static volatile uint32_t  m_lastTempUnitChangeFrameCounter;
      static volatile uint16_t  m_counterTempUnitChanged;

      static const unsigned long FramePeriodUs = 320;
      static const uint32_t SetupTrigUnitChangeFrameCounterMax =
        (SetupTrigUnitChangeStepMaxMs * 1000) / FramePeriodUs;
      static const uint32_t ResetErrorFrameCounter =
        (ResetErrorTimeMs * 1000) / FramePeriodUs;
    private:
      bool m_isopened;
      static IRAM_ATTR void clkRisingInterrupt();
      static IRAM_ATTR void holdRisingInterrupt();
  };


  //----------------------------------------------------------------------------
  inline uint8_t FrameDecoder::isPowerOn() const {

    return isLedOn (Power);
  }

  //----------------------------------------------------------------------------
  inline uint8_t FrameDecoder::isFilterOn() const {

    return isLedOn (Filter);
  }

  //----------------------------------------------------------------------------
  inline uint8_t FrameDecoder::isBubbleOn() const {

    return isLedOn (Bubble);
  }

  //----------------------------------------------------------------------------
  inline uint8_t FrameDecoder::isHeatReached() const {

    return isLedOn (HeatReached);
  }

  //----------------------------------------------------------------------------
  inline uint8_t FrameDecoder::isJetOn() const {

    return isLedOn (Jet);
  }

  //----------------------------------------------------------------------------
  inline uint8_t FrameDecoder::isSanitizerOn() const {

    return isLedOn (Sanitizer);
  }

}
