//
// SpaIot Server Alexa Client Example
//
// This example shows how to command a spa using Alexa.
// It use SinricPro library to connect to Alexa.
#include <cmath>
#include <SinricPro.h>
#include <SinricProThermostat.h>
#include "SpaAlexaClient.h"
#include "MyBoardSettings.h"

// To see debug messages on serial console when executing examples uncomment this line
// If PlatformIO serial monitor is used, DEBUG_ESP_PORT must be defined in platformio.ini
// #define DEBUG_ESP_PORT Serial0 
#include <spaiotdebug.h>

using namespace SpaIot;

// static, convert a string to a mode
const std::map<String,  SpaAlexaClient::Mode>  SpaAlexaClient::NameToMode = {
  { "OFF", OffMode},
  { "ECO", FilterMode},
  { "HEAT", HeatMode},
  { "AUTO", AutoMode}
};

// static, convert a mode to a string
const std::map<SpaAlexaClient::Mode, String>  SpaAlexaClient::ModeToName = {
  { OffMode, "OFF"},
  { FilterMode, "ECO"},
  { HeatMode, "HEAT"},
  { AutoMode, "AUTO"}
};

// list of events to subscribe to
std::initializer_list<Event::Type> SpaAlexaClient::SubsribedEvents = {
  Event::Type::PowerOn,
  Event::Type::FilterOn,
  Event::Type::HeaterOn,
  Event::Type::WaterTemp,
  Event::Type::DesiredTemp,
};


// Constructor
SpaAlexaClient::SpaAlexaClient () :
  SpaClient ("SpaAlexaClient"),
  isConnected (false), isHeaterOn (false), isFilterOn (false),
  thermostatMode (OffMode), desiredTemp (0) {

  // subscribe (SubsribedEvents);
}

// return the thermostat id
const String &SpaAlexaClient::deviceId() const {

  return settings.thermostatId;
}

// return true if the client is connected to the MQTT broker
bool SpaAlexaClient::isOpen() const {

  return isConnected;
}

// setting up the client
bool SpaAlexaClient::begin (const AlexaSettings &settings) {

  if (settings.isValid()) {

    this->settings = settings;
    SinricProThermostat &device = SinricPro[deviceId()];

    // PowerStateController
    device.onPowerState (onPowerState);

    // ThermostatController
    device.onTargetTemperature (onTargetTemperature);
    device.onAdjustTargetTemperature (onAdjustTargetTemperature);
    device.onThermostatMode (onThermostatMode);

    SinricPro.onConnected (onConnected);
    SinricPro.onDisconnected (onDisconnected);

    SinricPro.begin (settings.appKey, settings.appSecret);
    return true;
  }
  return false;
}

// Processing events from and to the spa
// return true if events have been processed
bool SpaAlexaClient::handle () {
  int processedEvents = 0;

  if (isOpen()) {
    Event event;
    SinricProThermostat &device = SinricPro[deviceId()];

    while (pullFromSpa (event)) {

      bool isOn = (event.value() == true && event.isBoolean());

      switch (event.type()) {
        case Event::Type::PowerOn:


          if (device.sendPowerStateEvent (isOn)) {

            TEST_PRINTF ("> Spa--> Power %s sent to Alexa", isOn ? "On" : "Off");
            if (event.value() == true) {

              // force the desired temperature to be published
              pushToSpa (Event (Event::Type::DesiredTemp, 0));
            }
            processedEvents++;
          }

          break;

        case Event::Type::FilterOn:
          isFilterOn = isOn;
          break;

        case Event::Type::HeaterOn:
          isHeaterOn = isOn;
          break;

        case Event::Type::DesiredTemp:
          if (device.sendTargetTemperatureEvent (event.value())) {

            desiredTemp = event.value();
            TEST_PRINTF ("> Spa--> Desired Temp. %d sent to Alexa", desiredTemp);
            processedEvents++;
          }
          break;

        case Event::Type::WaterTemp:
          if (device.sendTemperatureEvent (event.value())) {

            TEST_PRINTF ("> Spa--> Water Temp. %d sent to Alexa", event.value());
            processedEvents++;
          }
          break;

        default:
          break;
      }
    }

    if (isHeaterOn)  {
      if (thermostatMode != HeatMode) {

        if (sendThermostatMode (HeatMode)) {

          processedEvents++;
        }
      }
    }
    else  {

      if (sendThermostatMode (isFilterOn ? FilterMode : OffMode)) {

        processedEvents++;
      }
    }
  }

  SinricPro.handle();
  return processedEvents > 0;
}

//------------------------------------------------------------------------------
// Local user has changed the mode, send to Alexa
bool SpaAlexaClient::sendThermostatMode (SpaAlexaClient::Mode mode) {

  if (mode != thermostatMode) {
    const String &mn = ModeToName.at (mode);
    SinricProThermostat &device = SinricPro[deviceId()];

    thermostatMode = mode;
    TEST_PRINTF ("> Spa--> %s Mode sent to Alexa",  mn.c_str());
    return device.sendThermostatModeEvent (mn);
  }
  return false;
}

//------------------------------------------------------------------------------
//
//                                Callbacks
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void SpaAlexaClient::onConnected() {

  Alexa.isConnected = true;
  TEST_MESSAGE ("[Alexa] : Connected");
}

//------------------------------------------------------------------------------
void SpaAlexaClient::onDisconnected() {

  Alexa.isConnected = false;
  TEST_MESSAGE ("[Alexa] : Disconnected");
}

//------------------------------------------------------------------------------
// PowerStateController
// static
bool SpaAlexaClient::onPowerState (const String &deviceId, bool &state) {

  if (deviceId == Alexa.deviceId()) {

    TEST_PRINTF ("[Alexa: %s]: Power%s sent to spa", deviceId.c_str(), state ? "On" : "Off");
    Alexa.pushToSpa (Event (Event::Type::PowerOn, state));
  }
  return true; // request handled properly
}

//------------------------------------------------------------------------------
// ThermostatController
// static
// Callback executed when user requested to change the target temperature, e.g. using a slider in a mobile app
bool SpaAlexaClient::onTargetTemperature (const String &deviceId, float &temp) {
  uint16_t t = round (temp);

  if ( (t >= SpaIot::DesiredTempMin) &&
       (t <= SpaIot::DesiredTempMax))  {

    Alexa.pushToSpa (Event (Event::Type::SetDesiredTemp, t));
    TEST_PRINTF ("[Alexa: %s]: Target temperature %d sent to spa", deviceId.c_str(), t);
    Alexa.desiredTemp = t;
  }
  else {

    TEST_PRINTF ("[Alexa: %s]: Error ! Target temperature %.1f out of bound [%.1f,%.1f]",
                      deviceId.c_str(),  temp, DesiredTempMin, DesiredTempMax);
  }

  temp = Alexa.desiredTemp; // return the real target temperature
  return true; // request handled properly
}

//------------------------------------------------------------------------------
// static
// Callback executed when user requested to increase or decrease target temperature, e.g. using buttons in a mobile app
bool SpaAlexaClient::onAdjustTargetTemperature (const String &deviceId, float &tempDelta) {
  bool ret;
  float t = Alexa.desiredTemp + tempDelta;

  TEST_PRINTF ("[Alexa: %s]: Target temperature changed about %.1f", deviceId.c_str(), tempDelta);
  ret = onTargetTemperature (deviceId, t);
  tempDelta = Alexa.desiredTemp; // return absolute target temperature
  return ret;
}

//------------------------------------------------------------------------------
// static
// Callback executed when user requested to change thermostat mode, e.g. "Alexa, set thermostat mode to heat"
bool SpaAlexaClient::onThermostatMode (const String &deviceId, String &mode) {

  if (NameToMode.count (mode) > 0) {
    Mode m = NameToMode.at (mode);

    if (m != Alexa.thermostatMode) {
      bool isHeaterOn = ( (m & HeatMode) != 0);
      bool isFilterOn = ( (m & FilterMode) != 0);

      TEST_PRINTF ("[Alexa: %s]: Mode set to %s", deviceId.c_str(), mode.c_str());
      if (m == AutoMode) {

        // TODO: set vacation mode
        // Alexa.pushToSpa (Event (Event::Type::VacationOn, true));
      }
      if (isHeaterOn != Alexa.isHeaterOn) {

        Alexa.isHeaterOn = isHeaterOn;
        Alexa.pushToSpa (Event (Event::Type::HeaterOn, isHeaterOn));
        TEST_PRINTF ("[Alexa: %s]: Set Heater %s", deviceId.c_str(), isHeaterOn ? "On" : "Off");
      }
      if (isFilterOn != Alexa.isFilterOn) {

        Alexa.isFilterOn = isFilterOn;
        Alexa.pushToSpa (Event (Event::Type::FilterOn, isFilterOn));
        TEST_PRINTF ("[Alexa: %s]: Set Filter %s", deviceId.c_str(), isFilterOn ? "On" : "Off");
      }
      Alexa.thermostatMode = m;
    }
    return true;
  }
  TEST_PRINTF ("[Alexa: %s]: Thermostat mode %s not supported", deviceId.c_str(), mode.c_str());
  return false;
}

SpaAlexaClient Alexa; // singleton object