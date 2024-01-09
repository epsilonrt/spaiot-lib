//
// SpaIot Server Alexa Client Example
//
// This example shows how to command a spa using Alexa.
// It use SinricPro library to connect to Alexa.
// To control Alexa, say:
// - Alexa! start the spa
// - Alexa! stop the spa
// - Alexa! put the spa in automatic mode (filter ON, automatically rearmed)
// - Alexa! put the spa in economic mode (filter ON)
// - Alexa! put the spa in heating mode
// - Alexa! put the spa in stop mode
// - Alexa! what is the temperature in the spa?
// - Alexa! how much is the spa set to?
// - Alexa! set the spa temperature to 32°
// - Alexa! increase the spa temperature by 2°
#pragma once

#include <Arduino.h>
#include <SpaIot.h>

// The AlexaSettings class
// Contains the settings to connect to the Alexa service
class AlexaSettings {
  public:
    String appKey; // app key to connect to the Alexa service
    String appSecret; // app secret to connect to the Alexa service
    String  thermostatId; //
    // Default constructor
    AlexaSettings()  {}
    // Constructor with parameters
    AlexaSettings (const char *key, const char *secret, const char *id) :
      appKey (key), appSecret (secret), thermostatId (id) {}
    bool isValid() const {
      return !appKey.isEmpty() && !appSecret.isEmpty() && !thermostatId.isEmpty();
    }
};

// The SpaAlexaClient class
// Interconnects the spa and the Alexa service
class SpaAlexaClient : public SpaIot::SpaClient {
  public:
    // Constructor
    SpaAlexaClient ();

    // setting up the client and connecting to the spa and the Alexa service
    bool begin (const AlexaSettings &settings);

    // Processing events from and to the spa
    // return true if events have been processed
    bool handle ();

    // return true if the client is connected to the Alexa service
    bool isOpen() const;

  private:
    enum Mode {
      OffMode   = 0,
      FilterMode   = 0x001,
      HeatMode  = 0x010,
      AutoMode  = 0x101
    };

    bool sendThermostatMode (Mode mode);
    const String &deviceId() const;

    // callbacks
    static bool onPowerState (const String &deviceId, bool &state);
    static bool onTargetTemperature (const String &deviceId, float &temp);
    static bool onAdjustTargetTemperature (const String &deviceId, float &tempDelta);
    static bool onThermostatMode (const String &deviceId, String &mode);
    static void onConnected (void);
    static void onDisconnected (void);

  private:
    bool isConnected; // true if the client is connected to the Alexa service
    bool isHeaterOn; // true if the heater is on
    bool isFilterOn; // true if the filter is on
    Mode thermostatMode; // the last thermostat mode
    uint16_t desiredTemp; // the last desired temperature

    AlexaSettings settings; // the settings to connect to the Alexa service

    static const std::map<String, Mode> NameToMode;
    static const std::map<Mode, String> ModeToName;
    static std::initializer_list<SpaIot::Event::Type> SubsribedEvents; // list of events to subscribe to
};

// The SpaAlexaClient object, the only instance of SpaAlexaClient class
extern SpaAlexaClient Alexa;