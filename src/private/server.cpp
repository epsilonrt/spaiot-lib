/*
   SpaIot (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot <https://github.com/epsilonrt/spaiot-dev>

   SpaIot is the property of ϵIoT, and All rights are reserved. ϵIoT is the sole
   and exclusive owner, throughout the world. You may not copy, reproduce,
   transmit, or create derivative works without the express, prior, written
   permission of ϵIoT.
*/
#include <cmath>
#include "server_p.h"
#include "../client.h"
#include "../spaiotdebug.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                            Server
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  // static
  const std::map<Event::Type, uint8_t> Server::Private::PreviousStatesDefault = {
    {Event::Type::PowerOn, UnsetValue8},
    {Event::Type::FilterOn, UnsetValue8},
    {Event::Type::BubbleOn, UnsetValue8},
    {Event::Type::JetOn, UnsetValue8},
    {Event::Type::SanitizerOn, UnsetValue8},
    {Event::Type::HeaterOn, UnsetValue8},
    {Event::Type::HeatReached, UnsetValue8}
  };

  const std::map<Event::Type, uint16_t> Server::Private::PreviousValuesDefault = {
    {Event::Type::WaterTemp, UnsetValue16}, // read only
    {Event::Type::DesiredTemp, UnsetValue16},
    {Event::Type::SanitizerTime, UnsetValue16},
    {Event::Type::ErrorCode, 0} // read only
  };

  //----------------------------------------------------------------------------
  Server::Private::Private (Server *q) : ControlPanel::Private (q),
    settings (nullptr), previousPublishTime (0), previousStates (PreviousStatesDefault), previousValues (PreviousValuesDefault)
  {}

  //----------------------------------------------------------------------------
  Server::Private::Private (Server *q, const HardwareSettings &hwsettings) :
    ControlPanel::Private (q, hwsettings),
    settings (nullptr), previousPublishTime (0), previousStates (PreviousStatesDefault), previousValues (PreviousValuesDefault)
  {}

  //------------------------------------------------------------------------------
  // set previous state
  // if set power to false, set all other states to false
  // return true if the state is set
  bool Server::Private::setPreviousState (Event::Type type, uint8_t value) {
    PIMPL_Q (Server);

    if (previousStates.find (type) == previousStates.end()) {

      return false;
    }

    if ( (type == Event::Type::JetOn &&  q->hasLed (Key::Jet)) ||
         (type == Event::Type::SanitizerOn &&  q->hasLed (Key::Sanitizer)) ||
         (type != Event::Type::JetOn && type != Event::Type::SanitizerOn)) {


      if (type == Event::Type::PowerOn && value == false) {

        setPreviousState (Event::Type::BubbleOn, false);
        setPreviousState (Event::Type::HeaterOn, false);
        setPreviousState (Event::Type::HeatReached, false);
        setPreviousState (Event::Type::FilterOn, false);
        setPreviousState (Event::Type::JetOn, false);
        setPreviousState (Event::Type::SanitizerOn, false);
      }
      previousStates[type] = value;
    }

    return previousState (type) == value;
  }

  //------------------------------------------------------------------------------
  // set previous value
  // return true if the value is set
  bool Server::Private::setPreviousValue (Event::Type type, uint16_t value) {
    PIMPL_Q (Server);

    if (previousValues.find (type) == previousValues.end()) {

      return false;
    }

    if ( (type == Event::Type::SanitizerTime &&  q->hasLed (Key::Sanitizer)) ||
         (type != Event::Type::SanitizerTime)) {

      previousValues[type] = value;
    }
    return previousValue (type) == value;
  }

  //------------------------------------------------------------------------------
  // Return previous state for an event type
  uint8_t Server::Private::previousState (Event::Type type) {

    return previousStates[type];
  }

  //------------------------------------------------------------------------------
  // return previous state of the event
  uint8_t Server::Private::previousState (const Event &event) {

    return previousState (event.type());
  }

  //------------------------------------------------------------------------------
  // Sets the previous state of the event
  bool Server::Private::setPreviousState (const Event &event) {

    return setPreviousState (event.type(), event.value());
  }

  //------------------------------------------------------------------------------
  // Sets the previous value of the event
  bool Server::Private::setPreviousValue (const Event &event) {

    return setPreviousValue (event.type(), event.value());
  }

  //------------------------------------------------------------------------------
  // Return previous value for an event type
  uint16_t Server::Private::previousValue (Event::Type type) {

    return previousValues[type];
  }

  //------------------------------------------------------------------------------
  // return previous value of the event
  uint16_t Server::Private::previousValue (const Event &event) {

    return previousValue (event.type());
  }

  //------------------------------------------------------------------------------
  // send event to all clients
  void Server::Private::sendEvent (const Event &event) {

    for (auto &elmt : clients) {

      Client *client = elmt.second;
      client->write (event);
    }
  }

  //------------------------------------------------------------------------------
  // send state only if it is changed
  bool Server::Private::sendState (const Event &event) {

    if (setPreviousState (event)) {

      sendEvent (event);
      return true;
    }
    return false;
  }

  //------------------------------------------------------------------------------
  // send value only if it is changed
  bool Server::Private::sendValue (const Event &event) {

    if (setPreviousValue (event)) {

      sendEvent (event);
      return true;
    }
    return false;
  }
  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  Server::Server (Private &dd) : ControlPanel (dd)
  {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  // Default constructor
  // Call the protected constructor with private implementation
  Server::Server() :
    Server (*new Private (this))
  {}

  //----------------------------------------------------------------------------
  Server::Server (const HardwareSettings &hwsettings) :
    Server (*new Private (this, hwsettings))
  {}

  //----------------------------------------------------------------------------
  Server::Server (const String &hwSettingsName) :
    Server (*new Private (this, HardwareSettings::getFromRegister (hwSettingsName)))
  {}

  //------------------------------------------------------------------------------
  bool Server::begin (const ServerSettings &settings, unsigned long waitingTimeMs) {
    PIMPL_D (Server);

    SPAIOT_DBGP (PSTR ("[Server]: Load config. %s\n"), settings.spaModel().c_str());
    d->settings = &settings;
    for (auto &elmt : d->clients) {

      Client *client = elmt.second;
      if (!client->begin()) {
        SPAIOT_DBGP (PSTR ("%s:%d: Unable to start client\n"), __PRETTY_FUNCTION__, __LINE__);
        return false;
      }
    }
    ControlPanel::begin (settings.spaModel(), waitingTimeMs);
    return ControlPanel::isOpened();
  }

  //------------------------------------------------------------------------------
  void Server::addClient (Client &client) {
    PIMPL_D (Server);

    if (d->clients.count (client.className()) == 0) {

      d->clients[client.className()] = &client;
    }
  }

  //------------------------------------------------------------------------------
  void Server::removeClient (const String & className) {
    PIMPL_D (Server);

    if (d->clients.count (className) != 0) {

      d->clients.erase (className);
    }
  }

  //------------------------------------------------------------------------------
  void Server::removeClient (const Client &client) {

    removeClient (client.className());
  }

  //------------------------------------------------------------------------------
  bool Server::handle () {
    PIMPL_D (Server);
    bool ret = false;

    if (d->settings != nullptr) {
      unsigned long currentTime = millis();

      // read data from the client, and execute the command to the spa
      for (auto &elmt : d->clients) {
        Client *client = elmt.second;
        
        client->handle();

        // read data from the client
        while (client->available()) {
          Event event = client->read();

          if (event.value() != d->previousState (event)) { // if the state is changed

            if (d->setPreviousState (event) == true) { // the state is set successfully

              switch (event.type()) {

                case Event::Type::PowerOn: {
                  bool isOn = (event.value() == true);

                  if (setPower (isOn) == event.value()) {

                    SPAIOT_DBGP (PSTR ("[Server]: Power changed to %s\n"), isOn ? "on" : "off");
                    ret = true;
                  }
                  else {

                    SPAIOT_DBGP (PSTR ("%s:%d: Unable to set Power: %u"),
                                 __PRETTY_FUNCTION__, __LINE__, event.value());
                  }
                }
                break;

                case Event::Type::FilterOn:
                case Event::Type::BubbleOn:
                case Event::Type::JetOn:
                  if (event.type() == Event::Type::JetOn && !hasLed (Key::Jet)) {

                    break;
                  }
                  if (d->previousState (Event::Type::PowerOn) == true) {
                    bool isOn = (event.value() == true);

                    if (d->setKeyOn (event.type(), isOn) == event.value()) {

                      SPAIOT_DBGP (PSTR ("[Server]: %s changed to %s\n"), keyName (event.type()), isOn ? "on" : "off");
                      ret = true;
                    }
                    else {

                      SPAIOT_DBGP (PSTR ("%s:%d: Unable to set %s: %u"),
                                   __PRETTY_FUNCTION__, __LINE__, keyName (event.type()), event.value());
                    }
                  }
                  break;

                case Event::Type::HeaterOn:
                  if (d->previousState (Event::Type::PowerOn) == true) {
                    bool isOn = (event.value() == true);

                    if (setHeater (isOn) == event.value()) {

                      SPAIOT_DBGP (PSTR ("[Server]: %s changed to %s\n"), keyName (event.type()), isOn ? "on" : "off");
                      ret = true;
                    }
                    else {

                      SPAIOT_DBGP (PSTR ("%s:%d: Unable to set %s: %u"),
                                   __PRETTY_FUNCTION__, __LINE__, keyName (event.type()), event.value());
                    }
                  }
                  break;

                case Event::Type::SanitizerTime:
                  if (d->previousState (Event::Type::PowerOn) == true) {

                    if (setSanitizerTime (event.value())) {

                      SPAIOT_DBGP (PSTR ("[Server]: %s changed to %d\n"), keyName (event.type()), event.value());
                      ret = true;
                    }
                    else {

                      SPAIOT_DBGP (PSTR ("%s:%d: Unable to set %s: %u"),
                                   __PRETTY_FUNCTION__, __LINE__, keyName (event.type()), event.value());
                    }
                  }
                  break;

                case Event::Type::DesiredTemp:
                  if (d->previousState (Event::Type::PowerOn) == true) {

                    if (setDesiredTemp (event.value())) {

                      SPAIOT_DBGP (PSTR ("[Server]: %s changed to %d\n"), keyName (event.type()), event.value());
                      ret = true;
                    }
                    else {

                      SPAIOT_DBGP (PSTR ("%s:%d: Unable to set %s: %u"),
                                   __PRETTY_FUNCTION__, __LINE__, keyName (event.type()), event.value());
                    }
                  }
                  break;

                default:
                  break;
              }
            }
          }
        }
      }

      if (isReady()) { // if the spa is ready

        if ( (currentTime - d->previousPublishTime) >= d->settings->publishInterval()) { // if the publish interval is reached

          // Water Temp (read only)
          // Update Water Temp from local > ------------------------------------------
          uint16_t v = waterTemp();

          if ( (v != UnsetValue16) && (v != d->previousValue (Event::Type::WaterTemp))) {
            Event event (Event::Type::WaterTemp, v);

            if (d->sendValue (event)) {

              SPAIOT_DBGP (PSTR ("[Server]   : Water temp. changed to %d\n"), v);
            }
            else {

              SPAIOT_DBGP (PSTR ("%s:%d: Unable to send Water temp.: %u"),
                           __PRETTY_FUNCTION__, __LINE__, event.value());
            }

          }
          // < Update Water Temp from local ------------------------------------------

          // Sanitizer Time
          v = sanitizerTime();
          if (v != UnsetValue16 && v != d->previousValue (Event::Type::SanitizerTime)) {
            Event event (Event::Type::SanitizerTime, v);

            if (d->sendValue (event)) {

              SPAIOT_DBGP (PSTR ("[Server]   : Sanitized time changed to %d\n"), v);
            }
            else {

              SPAIOT_DBGP (PSTR ("%s:%d: Unable to send Sanitized time: %u"),
                           __PRETTY_FUNCTION__, __LINE__, event.value());
            }
          }

          // Error Code (read only)
          v = error();
          if (v != UnsetValue16 && v != d->previousValue (Event::Type::ErrorCode)) {
            Event event (Event::Type::ErrorCode, v);

            if (d->sendValue (event)) {

              SPAIOT_DBGP (PSTR ("[Server]   : Error code changed to %d\n"), v);
            }
            else {

              SPAIOT_DBGP (PSTR ("%s:%d: Unable to send error code: %u"),
                           __PRETTY_FUNCTION__, __LINE__, event.value());
            }
          }

          d->previousPublishTime = currentTime;
        }

        // review the states and send the changes to the client
        for (auto s : d->previousStates) {
          uint8_t isOn = (s.first == Event::Type::HeaterOn ? isHeaterOn() : isLedOn (s.first));

          if (d->previousState (s.first) != isOn) {
            Event event (s.first, isOn);

            d->sendState (event);
          }
        }
      }

    }
    return ret;
  }

}