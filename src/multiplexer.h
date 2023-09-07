/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include <initializer_list>
#include "buttoncontroller.h"

namespace SpaIot {

  /**
     @class Multiplexer
     @brief Analog multiplexer for buttons

      This class implements a multiplexer for buttons. It is a concrete class, and can be instantiated.
      It is a ButtonController, and can be used as such.
      This class is copyable and movable.
  */
  class Multiplexer : public ButtonController {
    public:
      /**
         @brief Constructor

         The constructor of Multiplexer takes a list of integers and an integer as input.
         The list of integers represents the selection pins that are connected to the multiplexer. The
         integer represents the pin that is connected to the inhibit pin

         @param spins a list of integers that represent the pins that are connected to
         the multiplexer, in ascending order of weight
         @param inhPin The pin number of the inhibit pin ((active in high level)
      */
      Multiplexer (const std::initializer_list<int> &spins, int inhPin);

      /**
         @brief Constructor with name

         The constructor of Multiplexer takes a list of integers and an integer as input.
         The list of integers represents the selection pins that are connected to the multiplexer. The
         integer represents the pin that is connected to the inhibit pin

         @param name The name of the multiplexer
         @param spins a list of integers that represent the pins that are connected to
         the multiplexer, in ascending order of weight
         @param inhPin The pin number of the inhibit pin ((active in high level)
      */
      Multiplexer (const String &name, const std::initializer_list<int> &spins, int inhPin);

      /**
         @brief Default constructor

         isEmpty() will return true after this constructor, used for copy or move
      */
      Multiplexer ();

      /**
        @copydoc ButtonController::ButtonController (const ButtonController &other)
      */
      Multiplexer (const Multiplexer &other);

      /**
         @copydoc ButtonController::operator= (const ButtonController &other)
      */
      Multiplexer &operator= (const Multiplexer &other);

      /**
         @copydoc ButtonController::ButtonController (ButtonController &&other)
      */
      Multiplexer (Multiplexer &&other);

      /**
         @copydoc ButtonController::operator= (const ButtonController &&other)
      */
      Multiplexer &operator= (Multiplexer &&other);

      /**
         @brief Given a key, return the corresponding value

         @param key The key of the parameter.
         @return The pin number for the given key.
      */
      int selectPin (int key) const;

      /**
         @brief It sets the pin number for the select pin of the given key.

         @param key the key to be used for the pin
         @param pin the pin number
      */
      void setSelectPin (int key, int pin);

      /**
        @brief Returns the number of multiplexer channels that are available

        That is the number of buttons that can be connected to the multiplexer
      */
      int size() const;

      // re-implemented methods from ButtonController
      virtual void clear();
      virtual bool isEmpty() const;
      virtual bool operator== (const ButtonController &other) const;
      virtual void begin();
      virtual void end();
      virtual int select (int button);
      virtual void deselect ();

    protected:
      class Private;
      Multiplexer (Private &dd);
    private:
      PIMPL_DECLARE_PRIVATE (Multiplexer)
  };

  /** @addtogroup HardwareSettings
     @{
     @name Multiplexers
     @{
  */

  /**
     @brief The first multiplexer for the Scip2 controller

     Connected as follows:
      - A -> GPIO4
      - B -> GPIO5
      - C -> GPIO15
      - INH -> GPIO16
      .
      may be accessed as "Scip2MuxA" in the Multiplexer Register.
      @see Scip2MuxB
  */
  extern Multiplexer Scip2MuxA;

  /**
     @brief The second multiplexer for the Scip2 controller

    Connected as follows:
      - A -> GPIO4
      - B -> GPIO5
      - C -> GPIO15
      - INH -> GPIO0
      .
      may be accessed as "Scip2MuxB" in the Multiplexer Register.
      @see Scip2MuxA
  */
  extern Multiplexer Scip2MuxB;

  /**
     @brief The first multiplexer for the SpaIot controller (ESP8266 version)

     Connected as follows:
      - A -> GPIO5
      - B -> GPIO4
      - C -> GPIO15
      - INH -> GPIO16
      .
      may be accessed as "SpaIot8266MuxA" in the Multiplexer Register.
      @see SpaIot8266MuxB
  */
  extern Multiplexer SpaIot8266MuxA;

  /**
     @brief The second multiplexer for the SpaIot controller (ESP8266 version)

      Connected as follows:
        - A -> GPIO5
        - B -> GPIO4
        - C -> GPIO15
        - INH -> GPIO0
        .
        may be accessed as "SpaIot8266MuxB" in the Multiplexer Register.
        @see SpaIot8266MuxA
  */
  extern Multiplexer SpaIot8266MuxB;

  /**
     @brief The first multiplexer for the SpaIot controller (ESP32 version)

      Connected as follows:
        - A -> GPIO27
        - B -> GPIO16
        - C -> GPIO17
        - INH -> GPIO33
        .
        may be accessed as "SpaIot32MuxA" in the Multiplexer Register.
        @see SpaIot32MuxB
  */
  extern Multiplexer SpaIot32MuxA;

  /**
     @brief The second multiplexer for the SpaIot controller (ESP32 version)

        Connected as follows:
          - A -> GPIO27
          - B -> GPIO16
          - C -> GPIO17
          - INH -> GPIO26
          .
          may be accessed as "SpaIot32MuxB" in the Multiplexer Register.
          @see SpaIot32MuxA
  */
  extern Multiplexer SpaIot32MuxB;

  /**
     @brief The first multiplexer for the SpaIot controller (ESP32S3 version)

      Connected as follows:
        - A -> GPIO4
        - B -> GPIO5
        - C -> GPIO6
        - INH -> GPIO7
        .
        may be accessed as "SpaIotS3MuxA" in the Multiplexer Register.
        @see SpaIotS3MuxB
  */
  extern Multiplexer SpaIotS3MuxA;

  /**
     @brief The second multiplexer for the SpaIot controller (ESP32S3 version)

        Connected as follows:
          - A -> GPIO4
          - B -> GPIO5
          - C -> GPIO6
          - INH -> GPIO15
          .
          may be accessed as "SpaIotS3MuxB" in the Multiplexer Register.
          @see SpaIotS3MuxA

  */
  extern Multiplexer SpaIotS3MuxB;

  /** @} */
  /** @} */
}
