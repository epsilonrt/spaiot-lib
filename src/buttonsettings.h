/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include <Arduino.h>
#include <map>
#include "buttoncontroller.h"

namespace SpaIot {

  class ButtonController;

  /**
     @class ButtonSettings
     @brief This class represents the settings of a button

      It is a concrete class, and can be instantiated.
      This class is copyable and movable.
  */
  class ButtonSettings {
    public:

      /**
         @brief The default constructor
      */
      ButtonSettings();

      /**
         @brief Destructor
      */
      virtual ~ButtonSettings();

      /**
         @brief The constructor for the ButtonSettings class takes two parameters
         @param controllerName the name of the controller in the ButtonController register (see ButtonController::addToRegister())
         @param buttonId the id of the button in the controller from SpaIot::Key
      */
      ButtonSettings (const String &controllerName, int buttonId);

      /**
         @brief The constructor for the ButtonSettings class takes two parameters
         @param controller the controller
         @param buttonId the id of the button in the controller from SpaIot::Key
      */
      ButtonSettings (ButtonController &controller, int buttonId);

      /**
        @brief Copy Constructor
      */
      ButtonSettings (const ButtonSettings &other);

      /**
         @brief Move Constructor
         after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      ButtonSettings (ButtonSettings &&other);

      /**
         @brief Sets the ButtonSettings object to be equal to @b other.
      */
      ButtonSettings &operator= (const ButtonSettings &other);

      /**
         @brief Move assignment
          after the move, @b other is null, other.clear() **must be called** if it is to be used again
      */
      ButtonSettings &operator= (ButtonSettings &&other);

      /**
         @brief Clears the ButtonSettings object
         After the call to this function, the ButtonSettings object is the same as one that has been default constructed
      */
      void clear();

      /**
         @brief Returns true if the object is null (i.e. it has not been initialized, d_ptr is null)
      */
      bool isNull() const;

      /**
         @brief Checks if all attributes are set to their default values (same as the default constructor)
      */
      bool isEmpty() const;

      /**
         @brief Checks if the two objects are equal
      */
      bool operator== (const ButtonSettings &other) const;

      /**
         @brief Checks if the two objects are not equal
      */
      bool operator!= (const ButtonSettings &other) const;

      /**
         @brief Returns the name of the controller
      */
      const String &controllerName() const;

      /**
         @brief Returns the id of the button
      */
      int id() const;

      /**
         @brief Sets the identifier of the button
      */
      void setId (int id);

      /**
         @brief Returns the button controller used by the button
      */
      const ButtonController &ctrl() const;

      /**
         @brief Returns the button controller used by the button
      */
      ButtonController &ctrl();

    protected:
      class Private;
      ButtonSettings (Private &dd);
      std::unique_ptr<Private> d_ptr;
    private:
      PIMPL_DECLARE_PRIVATE (ButtonSettings)
  };

  /** @addtogroup HardwareSettings
     @{
     @name Buttons
     @{
  */

  /**
     @brief ButtonSettings for the SSP Spa with Scip2 board

     Buttons are connected to the following pins:

     @code
      |  Button  |    Mux    | Bit |
      |:--------:|:---------:|:---:|
      |  Filter  | Scip2MuxA |  A1 |
      |  Bubble  | Scip2MuxA |  A3 |
      | TempDown | Scip2MuxA |  A7 |
      |   Power  | Scip2MuxB |  B2 |
      |  TempUp  | Scip2MuxB |  B4 |
      | TempUnit | Scip2MuxB |  B5 |
      |  Heater  | Scip2MuxB |  B7 |
     @endcode

    @see Scip2MuxA
    @see Scip2MuxB
  */
  extern const std::map<int, ButtonSettings> Scip2SspButtons;

  /**
     @brief ButtonSettings for the SSP Spa with SpaIot board (ESP8266 version)

     Buttons are connected to the following pins:

     @code
      |  Button  |       Mux      | Bit |
      |:--------:|:--------------:|:---:|
      |  Filter  | SpaIot8266MuxA |  A1 |
      |  Bubble  | SpaIot8266MuxA |  A3 |
      | TempDown | SpaIot8266MuxA |  A7 |
      |   Power  | SpaIot8266MuxB |  B2 |
      |  TempUp  | SpaIot8266MuxB |  B4 |
      | TempUnit | SpaIot8266MuxB |  B5 |
      |  Heater  | SpaIot8266MuxB |  B7 |
     @endcode

    @see SpaIot8266MuxA
    @see SpaIot8266MuxB
  */
  extern const std::map<int, ButtonSettings> SpaIot8266SspButtons;

  /**
     @brief ButtonSettings for the SSP Spa with SpaIot board (ESP32 version)

     Buttons are connected to the following pins:

     @code
      |  Button  |      Mux     | Bit |
      |:--------:|:------------:|:---:|
      |  Filter  | SpaIot32MuxA |  A1 |
      |  Bubble  | SpaIot32MuxA |  A3 |
      | TempDown | SpaIot32MuxA |  A7 |
      |   Power  | SpaIot32MuxB |  B2 |
      |  TempUp  | SpaIot32MuxB |  B4 |
      | TempUnit | SpaIot32MuxB |  B5 |
      |  Heater  | SpaIot32MuxB |  B7 |
     @endcode

    @see SpaIot32MuxA
    @see SpaIot32MuxB
  */
  extern const std::map<int, ButtonSettings> SpaIot32SspButtons;

  /**
     @brief ButtonSettings for the SSP Spa with SpaIot board (ESP32 S3 version)

     Buttons are connected to the following pins:

     @code
      |  Button  |      Mux     | Bit |
      |:--------:|:------------:|:---:|
      |  Filter  | SpaIotS3MuxA |  A1 |
      |  Bubble  | SpaIotS3MuxA |  A3 |
      | TempDown | SpaIotS3MuxA |  A7 |
      |   Power  | SpaIotS3MuxB |  B2 |
      |  TempUp  | SpaIotS3MuxB |  B4 |
      | TempUnit | SpaIotS3MuxB |  B5 |
      |  Heater  | SpaIotS3MuxB |  B7 |
     @endcode

    @see SpaIotS3MuxA
    @see SpaIotS3MuxB
  */
  extern const std::map<int, ButtonSettings> SpaIotS3SspButtons;

  /**
     @brief ButtonSettings for the SSP Spa with SpaIot Extended board with PCF8574A

      The buttons controller is a PCF8574A connected to 2 multiplexers 4051 as follows:
      - The selection pins {A, B, C} of the 2 multiplexers are connected
        respectively to {P0, P1, P2} on the PCF8574
      - The INH pin of [A7..A0] multiplexer is connected to P3 on the PCF8574
      - The INH pin of [B7..B0] multiplexer is connected to P4 on the PCF8574
      .

      This gives us the following values for the buttons of a SSP spa:
     @code
      |  Button  | BIT | P4 | P3 | P2 | P1 | P0 | DEC |
      |:--------:|:---:|:--:|:--:|:--:|:--:|:--:|:---:|
      |   Power  |  B2 |  0 |  1 |  0 |  1 |  0 |  10 |
      |  Filter  |  A1 |  1 |  0 |  0 |  0 |  1 |  17 |
      |  Bubble  |  A3 |  1 |  0 |  0 |  1 |  1 |  19 |
      |  Heater  |  B7 |  0 |  1 |  1 |  1 |  1 |  15 |
      |  TempUp  |  B4 |  0 |  1 |  1 |  0 |  0 |  12 |
      | TempDown |  A7 |  1 |  0 |  1 |  1 |  1 |  23 |
      | TempUnit |  B5 |  0 |  1 |  1 |  0 |  1 |  13 |
     @endcode

    @see SpaIotPcf8574Mux
  */
  extern const std::map<int, ButtonSettings> SpaIot8574SspButtons;

  /**
     @brief ButtonSettings for the SJB Spa with Scip2 board

     Buttons are connected to the following pins:

     @code
      |   Button  |    Mux    | Bit |
      |:---------:|:---------:|:---:|
      | Sanitizer | Scip2MuxA |  A0 |
      |    Jet    | Scip2MuxA |  A1 |
      |   Bubble  | Scip2MuxA |  A3 |
      |   Filter  | Scip2MuxA |  A7 |
      |  TempDown | Scip2MuxB |  B1 |
      |   Power   | Scip2MuxB |  B2 |
      |   TempUp  | Scip2MuxB |  B4 |
      |  TempUnit | Scip2MuxB |  B5 |
      |   Heater  | Scip2MuxB |  B7 |
     @endcode

    @see Scip2MuxA
    @see Scip2MuxB
  */
  extern const std::map<int, ButtonSettings> Scip2SjbButtons;

  /**
     @brief ButtonSettings for the SJB Spa with SpaIot board (ESP8266 version)

     Buttons are connected to the following pins:

     @code
      |   Button  |       Mux      | Bit |
      |:---------:|:--------------:|:---:|
      | Sanitizer | SpaIot8266MuxA |  A0 |
      |    Jet    | SpaIot8266MuxA |  A1 |
      |   Bubble  | SpaIot8266MuxA |  A3 |
      |   Filter  | SpaIot8266MuxA |  A7 |
      |  TempDown | SpaIot8266MuxB |  B1 |
      |   Power   | SpaIot8266MuxB |  B2 |
      |   TempUp  | SpaIot8266MuxB |  B4 |
      |  TempUnit | SpaIot8266MuxB |  B5 |
      |   Heater  | SpaIot8266MuxB |  B7 |
     @endcode

    @see SpaIot8266MuxA
    @see SpaIot8266MuxB
  */
  extern const std::map<int, ButtonSettings> SpaIot8266SjbButtons;

  /**
     @brief ButtonSettings for the SJB Spa with SpaIot board (ESP32 version)

     Buttons are connected to the following pins:

     @code
      |   Button  |      Mux     | Bit |
      |:---------:|:------------:|:---:|
      | Sanitizer | SpaIot32MuxA |  A0 |
      |    Jet    | SpaIot32MuxA |  A1 |
      |   Bubble  | SpaIot32MuxA |  A3 |
      |   Filter  | SpaIot32MuxA |  A7 |
      |  TempDown | SpaIot32MuxB |  B1 |
      |   Power   | SpaIot32MuxB |  B2 |
      |   TempUp  | SpaIot32MuxB |  B4 |
      |  TempUnit | SpaIot32MuxB |  B5 |
      |   Heater  | SpaIot32MuxB |  B7 |
     @endcode

    @see SpaIot32MuxA
    @see SpaIot32MuxB
  */
  extern const std::map<int, ButtonSettings> SpaIot32SjbButtons;

  /**
     @brief ButtonSettings for the SJB Spa with SpaIot board (ESP32 S3 version)

     Buttons are connected to the following pins:

     @code
      |   Button  |      Mux     | Bit |
      |:---------:|:------------:|:---:|
      | Sanitizer | SpaIotS3MuxA |  A0 |
      |    Jet    | SpaIotS3MuxA |  A1 |
      |   Bubble  | SpaIotS3MuxA |  A3 |
      |   Filter  | SpaIotS3MuxA |  A7 |
      |  TempDown | SpaIotS3MuxB |  B1 |
      |   Power   | SpaIotS3MuxB |  B2 |
      |   TempUp  | SpaIotS3MuxB |  B4 |
      |  TempUnit | SpaIotS3MuxB |  B5 |
      |   Heater  | SpaIotS3MuxB |  B7 |
     @endcode

    @see SpaIotS3MuxA
    @see SpaIotS3MuxB
  */
  extern const std::map<int, ButtonSettings> SpaIotS3SjbButtons;

  /**
     @brief ButtonSettings for the SJB Spa with SpaIot Extended board with PCF8574A

      The buttons controller is a PCF8574A connected to 2 multiplexers 4051 as follows:
      - The selection pins {A, B, C} of the 2 multiplexers are connected
        respectively to {P0, P1, P2} on the PCF8574
      - The INH pin of [A7..A0] multiplexer is connected to P3 on the PCF8574
      - The INH pin of [B7..B0] multiplexer is connected to P4 on the PCF8574
      .

      This gives us the following values for the buttons of a SSP spa:
     @code
      |  Button   | BIT | P4 | P3 | P2 | P1 | P0 | DEC |
      |:---------:|:---:|:--:|:--:|:--:|:--:|:--:|:---:|
      |   Power   |  B2 |  0 |  1 |  0 |  1 |  0 |  10 |
      |  Filter   |  A7 |  1 |  0 |  1 |  1 |  1 |  23 |
      |  Bubble   |  A3 |  1 |  0 |  0 |  1 |  1 |  19 |
      |  Heater   |  B7 |  0 |  1 |  1 |  1 |  1 |  15 |
      |  TempUp   |  B4 |  0 |  1 |  1 |  0 |  0 |  12 |
      | TempDown  |  B1 |  0 |  1 |  0 |  0 |  1 |  09 |
      | TempUnit  |  B5 |  0 |  1 |  1 |  0 |  1 |  13 |
      | Sanitizer |  A0 |  1 |  0 |  0 |  0 |  0 |  16 |
      |    Jet    |  A1 |  1 |  0 |  0 |  0 |  1 |  17 |
     @endcode

    @see SpaIotPcf8574Mux
  */
  extern const std::map<int, ButtonSettings> SpaIot8574SjbButtons;

  /** @} */
  /** @} */
}
