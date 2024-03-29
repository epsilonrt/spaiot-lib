/*
   SpaIot (c) by epsilonrt - epsilonrt@gmail.com
   This file is part of SpaIot <https://github.com/epsilonrt/spaiot-dev>

   SpaIot is the property of ϵIoT, and All rights are reserved. ϵIoT is the sole
   and exclusive owner, throughout the world. You may not copy, reproduce,
   transmit, or create derivative works without the express, prior, written
   permission of ϵIoT.
*/
#include <cmath>
#include "spaserver_p.h"
#include "unittest_p.h"
#include "../spaclient.h"

namespace SpaIot {

  //----------------------------------------------------------------------------
  //
  //                            SpaServer
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Private implementation

  //----------------------------------------------------------------------------
  // static
  const std::map<Event::Type, uint16_t> SpaServer::Private::PreviousValuesDefault = {
    {Event::Type::PowerOn, UnsetValue8},
    {Event::Type::FilterOn, UnsetValue8},
    {Event::Type::BubbleOn, UnsetValue8},
    {Event::Type::JetOn, UnsetValue8},
    {Event::Type::SanitizerOn, UnsetValue8},
    {Event::Type::HeaterOn, UnsetValue8},
    {Event::Type::HeatReached, UnsetValue8},
    {Event::Type::WaterTemp, UnsetValue16}, // read only
    {Event::Type::DesiredTemp, UnsetValue16},
    {Event::Type::SanitizerTime, UnsetValue16},
    {Event::Type::ErrorCode, 0} // read only
  };

  //----------------------------------------------------------------------------
  SpaServer::Private::Private (SpaServer *q) : ControlPanel::Private (q),
    settings (nullptr), previousPublishTime (0), previousValues (PreviousValuesDefault)
  {}

  //----------------------------------------------------------------------------
  SpaServer::Private::Private (SpaServer *q, const HardwareSettings &hwsettings) :
    ControlPanel::Private (q, hwsettings),
    settings (nullptr), previousPublishTime (0), previousValues (PreviousValuesDefault)
  {}

  //------------------------------------------------------------------------------
  // set previous value
  // if set power to false, set all other states to false
  // return true if the value is set
  bool SpaServer::Private::setPreviousValue (Event::Type type, uint16_t value) {
    PIMPL_Q (SpaServer);

    if (previousValues.count (type) == 0) {

      return false;
    }

    if ( (type != Event::Type::JetOn && type != Event::Type::SanitizerOn && type != Event::Type::SanitizerTime) ||
         (type == Event::Type::JetOn &&  q->hasLed (Key::Jet)) ||
         ( (type == Event::Type::SanitizerOn || type == Event::Type::SanitizerTime) &&  q->hasLed (Key::Sanitizer))) {

      previousValues[type] = value;
    }
    return previousValue (type) == value;
  }

  //------------------------------------------------------------------------------
  // Sets the previous state of the event
  bool SpaServer::Private::setPreviousValue (const Event &event) {

    return setPreviousValue (event.type(), event.value());
  }

  //------------------------------------------------------------------------------
  // Return previous value for an event type
  uint16_t SpaServer::Private::previousValue (Event::Type type) {

    if (type == Event::Type::SetDesiredTemp) {

      type = Event::Type::DesiredTemp;
    }
    if (previousValues.count (type) > 0) {

      return previousValues[type];
    }
    TEST_PRINTF ("%s:%d: Unknown event type %d", __PRETTY_FUNCTION__, __LINE__, type);
    return UnsetValue16;
  }

  //------------------------------------------------------------------------------
  // return previous value of the event
  uint16_t SpaServer::Private::previousValue (const Event &event) {

    return previousValue (event.type());
  }

  //------------------------------------------------------------------------------
  // send value only if it is changed
  bool SpaServer::Private::sendValue (const Event &event) {

    if (setPreviousValue (event)) {

      for (auto &elmt : clients) {

        SpaClient *client = elmt.second;
        TEST_PRINTF ("%s:%d: Send event %s to %s",
                     __PRETTY_FUNCTION__, __LINE__, event.toString().c_str(), client->className().c_str());
        client->write (event);
      }
      return true;
    }
    return false;
  }

  //----------------------------------------------------------------------------
  // Protected constructor with private implementation
  SpaServer::SpaServer (Private &dd) : ControlPanel (dd)
  {}

  //----------------------------------------------------------------------------
  // API

  //----------------------------------------------------------------------------
  // Default constructor
  // Call the protected constructor with private implementation
  SpaServer::SpaServer() :
    SpaServer (*new Private (this))
  {}

  //------------------------------------------------------------------------------
  bool SpaServer::begin (const ServerSettings &settings, unsigned long waitingTimeMs) {

    if (HardwareSettings::registerContains (settings.spaModel())) {
      PIMPL_D (SpaServer);

      d->settings = &settings;
      ControlPanel::begin (settings.spaModel(), waitingTimeMs);
    }
    else {

      TEST_PRINTF ("%s:%d: Unknown spa model %s",
                   __PRETTY_FUNCTION__, __LINE__, settings.spaModel().c_str());
    }
    return ControlPanel::isOpen();
  }

  //------------------------------------------------------------------------------
  void SpaServer::end () {

    if (isOpen()) {
      PIMPL_D (SpaServer);

      for (auto &elmt : d->clients) {
        SpaClient *client = elmt.second;

        client->end();
      }
      ControlPanel::end();
      d->settings = nullptr;
    }
  }

  //------------------------------------------------------------------------------
  bool SpaServer::handle () {
    bool isEventsProcessed = false;

    if (isOpen() && isReady()) { // if the spa is ready
      PIMPL_D (SpaServer);
      unsigned long now = millis();

      if ( (now - d->previousPublishTime) >= d->settings->publishInterval()) { // if the publish interval is reached

        // Water Temp (read only)
        // Update Water Temp from local > ------------------------------------------
        uint16_t v = waterTemp();

        if ( (v != UnsetValue16) && (v != d->previousValue (Event::Type::WaterTemp))) {
          Event event (Event::Type::WaterTemp, v);

          if (d->sendValue (event)) {

            TEST_PRINTF ("%s:%d: Water temp. changed to %d",
                         __PRETTY_FUNCTION__, __LINE__, v);

          }
          else {

            TEST_PRINTF ("%s:%d: Unable to send Water temp.: %d",
                         __PRETTY_FUNCTION__, __LINE__, event.value());
          }

        }
        // < Update Water Temp from local ------------------------------------------

        // Update Desired Temp from local > ------------------------------------------
        v = desiredTemp();

        if ( (v != UnsetValue16) && (v != d->previousValue (Event::Type::DesiredTemp))) {
          Event event (Event::Type::DesiredTemp, v);

          if (d->sendValue (event)) {

            TEST_PRINTF ("%s:%d: Desired temp. changed to %d",
                         __PRETTY_FUNCTION__, __LINE__, v);

          }
          else {

            TEST_PRINTF ("%s:%d: Unable to send Desired temp.: %d",
                         __PRETTY_FUNCTION__, __LINE__, event.value());
          }

        }
        // < Update Desired Temp from local ------------------------------------------

        // Sanitizer Time
        v = sanitizerTime();
        if (v != UnsetValue16 && v != d->previousValue (Event::Type::SanitizerTime)) {
          Event event (Event::Type::SanitizerTime, v);

          if (d->sendValue (event)) {

            TEST_PRINTF ("%s:%d: Sanitized time changed to %d",
                         __PRETTY_FUNCTION__, __LINE__, v);

          }
          else {

            TEST_PRINTF ("%s:%d: Unable to send Sanitized time: %d",
                         __PRETTY_FUNCTION__, __LINE__, event.value());
          }
        }

        // Error Code (read only)
        v = error();
        if (v != UnsetValue16 && v != d->previousValue (Event::Type::ErrorCode)) {
          Event event (Event::Type::ErrorCode, v);

          if (d->sendValue (event)) {

            TEST_PRINTF ("%s:%d: Error code changed to %d",
                         __PRETTY_FUNCTION__, __LINE__, v);

          }
          else {

            TEST_PRINTF ("%s:%d: Unable to send error code: %d",
                         __PRETTY_FUNCTION__, __LINE__, event.value());

          }
        }

        d->previousPublishTime = now;
      }

      // review the values and send the changes to the client
      for (auto s : d->previousValues) {
        Event::Type type = s.first;

        if (Event::TypeIsBoolean (type)) { // if the event is boolean (on/off)
          uint8_t isOn = (type == Event::Type::HeaterOn ? isHeaterOn() : isLedOn (type));

          if (d->previousValue (type) != isOn) {

            TEST_PRINTF ("%s:%d:>>>>>>>:  previous  %d, current %d",
                         __PRETTY_FUNCTION__, __LINE__, d->previousValue (type), isOn);
            d->sendValue (Event (type, isOn));
          }
        }
      }

      // read data from the client, and execute the command to the spa
      for (auto &elmt : d->clients) {
        SpaClient *client = elmt.second;

        if (client->handle() || client->available() > 0) {

          isEventsProcessed = true;

          // read data from the client
          while (client->available() > 0) {

            Event event = client->read();
            if (event.value() != d->previousValue (event)) { // if the value is changed


              switch (event.type()) {

                case Event::Type::PowerOn: {
                  bool isOn = (event.value() == true);

                  if (setPower (isOn) == event.value()) {

                    TEST_PRINTF ("%s:%d: Power changed to %s",
                                 __PRETTY_FUNCTION__, __LINE__, isOn ? "On" : "Off");

                  }
                  else {

                    TEST_PRINTF ("%s:%d: Unable to set Power: %d",
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
                  if (d->previousValue (Event::Type::PowerOn) == true) {
                    bool isOn = (event.value() == true);

                    if (d->setKeyOn (event.type(), isOn) == event.value()) {

                      TEST_PRINTF ("%s:%d: %s changed to %s",
                                   __PRETTY_FUNCTION__, __LINE__, Event::typeToString (event.type()).c_str(), isOn ? "On" : "Off");
                    }
                    else {

                      TEST_PRINTF ("%s:%d: Unable to set %s: %d",
                                   __PRETTY_FUNCTION__, __LINE__, Event::typeToString (event.type()).c_str(), event.value());

                    }
                  }
                  break;

                case Event::Type::HeaterOn:
                  if (d->previousValue (Event::Type::PowerOn) == true) {
                    bool isOn = (event.value() == true);

                    if (setHeater (isOn) == event.value()) {

                      TEST_PRINTF ("%s:%d: %s changed to %s",
                                   __PRETTY_FUNCTION__, __LINE__, Event::typeToString (event.type()).c_str(), isOn ? "On" : "Off");
                    }
                    else {

                      TEST_PRINTF ("%s:%d: Unable to set %s: %d",
                                   __PRETTY_FUNCTION__, __LINE__, Event::typeToString (event.type()).c_str(), event.value());

                    }
                  }
                  break;

                case Event::Type::SanitizerTime:
                  if (d->previousValue (Event::Type::PowerOn) == true) {
                    // TODO: check with a real spa
                    if (setSanitizerTime (event.value())) {

                      TEST_PRINTF ("%s:%d: %s changed to %d",
                                   __PRETTY_FUNCTION__, __LINE__, Event::typeToString (event.type()).c_str(), event.value());
                      d->sendValue (Event (Event::Type::SanitizerTime, sanitizerTime()));
                    }
                    else {

                      TEST_PRINTF ("%s:%d: Unable to set %s: %d",
                                   __PRETTY_FUNCTION__, __LINE__, Event::typeToString (event.type()).c_str(), event.value());
                    }
                  }
                  break;

                case Event::Type::SetDesiredTemp:
                  if (d->previousValue (Event::Type::PowerOn) == true) {

                    if (setDesiredTemp (event.value())) {

                      TEST_PRINTF ("%s:%d: %s changed to %d",
                                   __PRETTY_FUNCTION__, __LINE__, Event::typeToString (event.type()).c_str(), event.value());
                      d->sendValue (Event (Event::Type::DesiredTemp, desiredTemp()));
                    }
                    else {

                      TEST_PRINTF ("%s:%d: Unable to set %s: %d",
                                   __PRETTY_FUNCTION__, __LINE__, Event::typeToString (event.type()).c_str(), event.value());
                    }
                  }
                  break;

                case Event::Type::DesiredTemp:
                  if (d->previousValue (Event::Type::PowerOn) == true) {
                    TEST_MESSAGE ("DesiredTemp event received");
                    uint16_t v = waitForDesiredTemp ();
                    if (v != UnsetValue16) {

                      TEST_PRINTF ("%s:%d: Get %s: %d'C",
                                   __PRETTY_FUNCTION__, __LINE__, Event::typeToString (event.type()).c_str(), v);
                      d->sendValue (Event (Event::Type::DesiredTemp, v));
                    }
                    else {

                      TEST_PRINTF ("%s:%d: Unable to get %s",
                                   __PRETTY_FUNCTION__, __LINE__, Event::typeToString (event.type()).c_str());
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

    }
    return isEventsProcessed;
  }

  //------------------------------------------------------------------------------
  const ServerSettings *SpaServer::settings() const {
    PIMPL_D (const SpaServer);

    return d->settings;
  }

  //------------------------------------------------------------------------------
  bool SpaServer::addClient (SpaClient &client) {
    PIMPL_D (SpaServer);

    if (d->clients.count (client.className()) == 0) {

      d->clients[client.className()] = &client;
      return true;
    }
    return false;
  }

  //------------------------------------------------------------------------------
  int SpaServer::clientCount () const {
    PIMPL_D (const SpaServer);

    return d->clients.size();
  }

  //------------------------------------------------------------------------------
  SpaClient *SpaServer::client (const String &className) const {
    PIMPL_D (const SpaServer);

    if (d->clients.count (className) != 0) {

      return d->clients.at (className);
    }
    return nullptr;
  }

  //------------------------------------------------------------------------------
  bool SpaServer::removeClient (const String &className) {
    PIMPL_D (SpaServer);

    if (d->clients.count (className) != 0) {

      d->clients.erase (className);
      return true;
    }
    return false;
  }

  //------------------------------------------------------------------------------
  bool SpaServer::removeClient (const char *className) {

    return removeClient (String (className));
  }

  //------------------------------------------------------------------------------
  bool SpaServer::removeClient (const SpaClient &client) {

    return removeClient (client.className());
  }

}
