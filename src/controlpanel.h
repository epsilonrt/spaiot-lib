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

#include "hardwaresettings.h"
#include "framedecoder.h"
#include "button.h"

namespace SpaIot {

  /**
   * @class ControlPanel
   * @brief Control Panel
   *
   * This class is the programming interface of the application (API).
   * It allows to monitor and control the spa.\n
   * It is a singleton class that can only be instantiated through getInstance()
   */
  class ControlPanel : public FrameDecoder {
    public:
      /**
       * @brief Create the control panel singleton with the provided settings
       * @param hwsettings Description of hardware settings
       * @return reference on the created singleton
       */
      static ControlPanel & singleton (const HardwareSettings & hwsettings);
      /**
       * @overload
       * @param hwSettingsName configuration name in the register of hardware settings
       */
      static ControlPanel & singleton (const String & hwSettingsName);
      /**
       * @overload
       */
      static ControlPanel & singleton ();
      /**
       * @brief Create the control panel instance with the provided settings
       * @param hwsettings Description of hardware settings
       * @return pointer on the created instance, nullptr if failure
       * @deprecated This function will be deleted during the next major version, use singleton() instead !
       */
      static ControlPanel * getInstance (const HardwareSettings & hwsettings);
      /**
       * @overload
       * @param hwSettingsName configuration name in the register of hardware settings
       * @deprecated This function will be deleted during the next major version, use singleton() instead !
       */
      static ControlPanel * getInstance (const String & hwSettingsName);
      /**
       * @overload
       * @deprecated This function will be deleted during the next major version, use singleton() instead !
       */
      static ControlPanel * getInstance ();
      /**
       * @brief Configures each of the buttons and initializes and connect with the spa
       *
       * \c isOpened() lets you know if the connection has been successfully completed
       */
      void begin (unsigned long waitingTimeMs = BeginWaitingTimeMs);
      /**
       * @brief Configures each of the buttons and initializes and connect with the spa
       *
       * \c isOpened() lets you know if the connection has been successfully completed
       * @param hwsettings Description of hardware settings
       */
      void begin (const HardwareSettings & hwsettings, unsigned long waitingTimeMs = BeginWaitingTimeMs);
      /**
       * @brief Configures each of the buttons and initializes and connect with the spa
       *
       * \c isOpened() lets you know if the connection has been successfully completed
       * @param hwSettingsName configuration name in the register of hardware settings
       */
      void begin (const String & hwSettingsName, unsigned long waitingTimeMs = BeginWaitingTimeMs);
      /**
       * @brief Indicates whether the connection with the spa is established.
       *
       * @return If \c begin() succeeded and everything works returns true.
       * Returns false if the connection to the spa is not established and no frame is received.
       */
      bool isOpened() const;
      /**
       * @brief Check if the hardware configuration has the button
       * @param key button identification key in the possible values of SpaIot::Key
       * @return true if the button exists, false otherwise
       */
      bool hasButton (int key) const;
      /**
       * @brief Button
       * @param key button identification key in the possible values of SpaIot::Key
       * @return refernce on the button
       */
      Button & button (int key);
      /**
       * @brief Press and release a button
       * @param key button identification key in the possible values of SpaIot::Key
       * @return true if the button exists, false otherwise
       */
      bool pushButton (int key);
      /**
       * @brief Start or stop the spa
       * @param v true for ON, false for OFF
       * @return the state returned by isPowerOn()
       */
      uint8_t setPower (bool v = true);
      /**
       * @brief Start or stop the water filtration
       * @param v true for ON, false for OFF
       * @return the state returned by isFilterOn()
       */
      uint8_t setFilter (bool v = true);
      /**
       * @brief Start or stop water heating
       * @param v true for ON, false for OFF
       * @return the state returned by isHeaterOn()
       */
      uint8_t setHeater (bool v = true);
      /**
       * @brief Start or stop the the bubble generator
       * @param v true for ON, false for OFF
       * @return the state returned by isBubbleOn()
       */
      uint8_t setBubble (bool v = true);
      /**
       * @brief Start or stop the water jets
       * @param v true for ON, false for OFF
       * @return the state returned by isJetOn()
       */
      uint8_t setJet (bool v = true);
      /**
       * @brief Setting the desired water temperature
       * @param temp temperature in ??C
       * @return
       */
      bool setDesiredTemp (uint16_t temp);
      /**
       * @brief Set the water sanitation time
       *
       * @param time Sanitation time in hours, the possible values are {0, 3, 5, 8}. 0 Disables sanitation.
       * @return true if the setting has been made, false if the requested value is invalid.
       */
      bool setSanitizerTime (uint16_t time);
      /**
       * @brief Press the TempUp button to read the desired temperature
       * @param MaxWaitingTimeMs Maximum waiting time in milliseconds
       * @return  Water temperature desired in ??C, UnsetValue16 if it has not
       * been determined yet.
       */
      uint16_t waitForDesiredTemp (unsigned long MaxWaitingTimeMs = 5000);
      /**
       * @brief Close the FrameDecoder
       */
      void end();
      /**
       * @brief Destructor
       *
       * Call end() and delete m_instance
       */
      ~ControlPanel();
      /**
       * @brief Button settings provides
       * @return Constant reference on the [std::map](https://en.cppreference.com/w/cpp/container/map)  containing the button settings.
       * The different key values are defined by SpaIot::Key
       */
      const std::map <int, ButtonSettings> buttonSettings() const;

    protected:
      /**
       * @brief Buttons list
       * @return Constant reference on the [std::map](https://en.cppreference.com/w/cpp/container/map)  containing the buttons. 
       * The different key values are defined by SpaIot::Key
       */
      const std::map <int, Button> & buttons() const;
      ControlPanel (const HardwareSettings & hwsettings);
      ControlPanel ();
      uint8_t setKeyOn (int key, bool v);
      bool makeButtons();

    private:
      std::map <int, ButtonSettings> m_btnsettings;
      bool m_isopened;
      std::map <int, Button> m_button;
      static ControlPanel * m_instance;
  };

}
