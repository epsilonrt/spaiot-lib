//
// SpaIot Server MQTT Client Example
//
// This example shows how to connect a spa to a MQTT broker.
#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SpaIot.h>

// Define the serial console, depending on the platform
#if defined(ESP8266)
#define Console Serial

#elif defined(ARDUINO_LOLIN_S3)
// Serial  = OTG USB
// Serial0 = UART0 -> Default Pin GPIO18 (RX0) and GPIO17 (TX0), connected to USB-UART (CH340)
// Serial1 = UART1 -> Default Pin GPIO18 (RX1) and GPIO17 (TX1)
#define Console Serial0 

#elif defined(ESP32)
#define Console Serial

#else
#error unsupported platform
#endif

// The MqttSettings class
// Contains the settings to connect to the MQTT broker
class MqttSettings {
  public:
    String broker; // IP address or DNS name of the MQTT broker
    int port; // port of the MQTT broker
    String username; // username to connect to the MQTT broker, empty if no credentials are required
    String password; // password to connect to the MQTT broker
    String topic; // prefix topic used to communicate with the spa
    // Default constructor
    MqttSettings() : port (1883), topic("epsilonrt/spaiot") {}
    // Constructor with parameters
    MqttSettings (const char *mqtt_broker, const char *mqtt_username, const char *mqtt_password, const char *mqtt_topic = "epsilonrt/spaiot") :
      broker (mqtt_broker), port (1883), username (mqtt_username), password (mqtt_password), topic (mqtt_topic) {}
    MqttSettings (const char *mqtt_broker, const char *mqtt_topic = "epsilonrt/spaiot") :
      broker (mqtt_broker), port (1883), topic (mqtt_topic) {}
};

// The SpaMqttClientClass class
// Interconnects the spa and the MQTT broker
class SpaMqttClientClass : public SpaIot::SpaClient {
  public:
    // Constructor
    SpaMqttClientClass ();

    // setting up the client and connecting to the spa and the MQTT broker
    void begin (Client &client, const MqttSettings &settings);

    // reconnecting to the MQTT broker
    void reconnect ();

    // Processing events from and to the spa
    // return true if events have been processed
    bool handle ();

    // return true if the client is connected to the MQTT broker
    bool isOpen() const;

  private:
    // callback function
    // called when a message mqtt arrives
    static void mqttCallback (char *topic, byte *payload, unsigned int length);

  private:
    mutable PubSubClient mqtt; // the MQTT client, mutable because PubSubClient::connected() is not const
    MqttSettings settings; // the settings to connect to the MQTT broker
    static const std::map<String, SpaIot::Event::Type> MqttCommandToType; // map MQTT command to event type   
    static const std::map<SpaIot::Event::Type, String> TypeToMqttTopic; // map event type to MQTT topic
};

// The SpaMqttClient object, the only instance of SpaMqttClientClass class
extern SpaMqttClientClass SpaMqttClient;