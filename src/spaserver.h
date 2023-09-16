/*
   SpaIot Library (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>

   SPDX-License-Identifier: BSD-3-Clause

   SpaIot library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;
*/
#pragma once

#include "serversettings.h"
#include "controlpanel.h"

namespace SpaIot {
  class SpaClient;
  /**
     @brief SpaServer class

     This class is used to send and receive events to/from, control and monitor the spa from the external world.
     The external world is represented by a client, which can be a web browser, a mobile app or a desktop application.
     
     Here is a schematic representation of the communication between the spa and the clients:
     @verbatim
     +-----------+               FiFo                     +-----------+
     |           | write() --> OOOOOOOO --> pullFromSpa() |           |
     | SpaServer |                                        | Protected | <---> External world (MQTT, HTTP, Alexa, ...)
     |   (SPA)   | read()  <-- OOOOOOOO <---- pushToSpa() |    API    |       implements the protocol with an external library
     +-----------+               FiFo                     +-----------+
                 [ ----------------   SpaClient   ----------------   ] [ --- External library (PubSubClient, SinricPro...) ---]
     @endverbatim

     The communication between server and clients is asynchronous, clients can be connected or disconnected at any time.
     When the spa receives a command from a client, it executes it, then if the command has modified the state of the spa, it sends an event to all connected clients.
     @warning Thus a client must not block its process to wait for an event from the spa or the system will be blocked.
  */
  class SpaServer : public ControlPanel {
    public:
      /**
         @brief Default constructor
      */
      SpaServer ();

      /**
         @brief Add a client to the server

         Must be called in the setup() function before \c begin().
         @param client SpaClient to add
         @return true if the client is added successfully, false otherwise
      */
      bool addClient (SpaClient &client);

      /**
         @brief Return the number of clients added to the server
      */
      int clientCount () const;

      /**
          @brief Return the client with the specified className

          @param className the name of the client class
          @return the client with the specified className, or nullptr if not found
      */
      SpaClient *client (const String &className) const;

      /**
         @brief Remove a client from the server

         @param className the name of the client class
      */
      bool removeClient (const String &className);

      /**
         @brief Remove a client from the server

         @param className the name of the client class
      */
      bool removeClient (const char * className);

      /**
         @brief Remove a client from the server

         @param client the client to remove
      */
      bool removeClient (const SpaClient &client);

      /**
         @brief Start the server

         Configures each of the buttons and initializes and connect with the spa.
          Must be called in the setup() function after \c addClient().
         @warning All clients must be added and its begin() method must be called before calling this method.
         @param settings SpaServer settings, contains the name of the spa hardware settings
                that must be stored in the registry beforehand.
         @return true if the server is started successfully, false otherwise
      */
      bool begin (const ServerSettings &settings, unsigned long waitingTimeMs = BeginWaitingTimeMs);

      /**
         @brief Stop the server

         This method must be called in the main loop when the server is no longer needed.
         It closes the server and stops the server services and the client services with SpaClient::end().
      */
      void end ();

      /**
         @brief Process spa events and handle client requests

         This method must be called periodically in the main loop.
         @return true if an event has been processed, false otherwise
      */
      bool handle ();

      /**
         @brief Returns the SpaServer Settings

         @return the SpaServer Settings, or nullptr if not set
      */
      const ServerSettings   *settings() const;

      /**
         @brief Set the Vacation Mode

         When the vacation mode is activated, the filtration is always active.
         If the spa timer stops the filtration, the filtration is restarted.
         The user can not stop the filtration, the only way to stop it is to deactivate the vacation mode or to stop the spa.
         @param v  true for ON, false for OFF
      */
      void setVacationMode (bool v = true);

      /**
         @brief Returns the state of the vacation mode
         q
         @return true if the vacation mode is ON, false if the vacation mode is OFF, UnsetValue8 if not set
      */
      uint8_t isVacationMode() const;

    protected:
      class Private;
      SpaServer (Private &dd);
    private:
      PIMPL_DECLARE_PRIVATE (SpaServer)
  };

}