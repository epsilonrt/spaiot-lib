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

#include "serversettings.h"
#include "controlpanel.h"

namespace SpaIot {
  class Client;
  /**
     @brief Server class

     This class is used to send and receive events to/from, control and monitor the spa from the external world.
     The external world is represented by a client, which can be a web browser, a mobile app or a desktop application.
  */
  class Server : public ControlPanel {
    public:
      /**
         @brief Default constructor
      */
      Server ();

      /**
         @brief Constructor with hardware settings
         @param hwsettings Hardware settings
      */
      Server (const HardwareSettings &hwsettings);
      Server (const String &hwSettingsName);


      /**
         @brief Add a client to the server

         Must be called in the setup() function before \c begin().
         @param client Client to add
      */
      void addClient (Client &client);

      void removeClient (const String & className);
      void removeClient (const Client &client);

      /**
         @brief Start the server and all clients

         Configures each of the buttons and initializes and connect with the spa

         @param settings Server settings, contains the name of the spa hardware settings 
                that must be stored in the registry beforehand.
         @return true if the server is started successfully, false otherwise
      */
      bool begin (const ServerSettings &settings, unsigned long waitingTimeMs = BeginWaitingTimeMs);

      /**
         @brief Process spa events and handle client requests

         This method must be called periodically in the main loop.
         @return true if an event has been processed, false otherwise
      */
      bool handle ();

    protected:
      class Private;
      Server (Private &dd);
    private:
      PIMPL_DECLARE_PRIVATE (Server)
  };

}