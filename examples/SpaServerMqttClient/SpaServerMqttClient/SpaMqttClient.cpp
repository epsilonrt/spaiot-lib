//
// SpaIot Server MQTT Client Example
//
// This example shows how to connect a spa to a MQTT broker.

#include "SpaMqttClient.h"
#include "MyBoardSettings.h"

using namespace SpaIot;

// Map to convert command topic to event
const std::map <String, Event::Type> SpaMqttClientClass::MqttCommandToType = {
  {"power", Event::Type::PowerOn},
  {"filter", Event::Type::FilterOn},
  {"bubble", Event::Type::BubbleOn},
  {"heater", Event::Type::HeaterOn},
  {"heat_reached", Event::Type::HeatReached},
  {"desired_temp", Event::Type::SetDesiredTemp}
};

const std::map<SpaIot::Event::Type, String> SpaMqttClientClass::TypeToMqttTopic = {
  {Event::Type::PowerOn, "power"},
  {Event::Type::FilterOn, "filter"},
  {Event::Type::BubbleOn, "bubble"},
  {Event::Type::HeaterOn, "heater"},
  {Event::Type::HeatReached, "heat_reached"},
  {Event::Type::DesiredTemp, "desired_temp"},
  {Event::Type::WaterTemp, "water_temp"}
};

// Constructor
SpaMqttClientClass::SpaMqttClientClass () : SpaClient ("SpaMqttClientClass") {}


// setting up the client
void SpaMqttClientClass::begin (Client &client, const MqttSettings &settings) {

  this->settings = settings;
  mqtt.setClient (client);
  mqtt.setServer (settings.broker.c_str(), settings.port);
  mqtt.setCallback (SpaMqttClientClass::mqttCallback);
  reconnect();
}

// return true if the client is connected to the MQTT broker
bool SpaMqttClientClass::isOpen() const {

  return mqtt.connected();
}

// connecting to the broker
void SpaMqttClientClass::reconnect () {
  String client_id = className();
  client_id += String (WiFi.macAddress());

  while (!mqtt.connected()) {
    bool connected = false;

    Console.printf ("The client %s connects to the MQTT broker\n", client_id.c_str());
    if (settings.username.isEmpty()) {

      // no credentials
      connected = mqtt.connect (client_id.c_str());
    }
    else {

      connected = mqtt.connect (client_id.c_str(), settings.username.c_str(), settings.password.c_str());
    }
    if (connected) {

      Console.println ("MQTT broker connected");
    }
    else {

      Console.print ("failed with state ");
      Console.print (mqtt.state());
      delay (2000);
    }
  }

  for (auto item : MqttCommandToType) {
    const String &topic = item.first;
    String command = settings.topic + "/command/" + topic;

    Console.printf ("The client %s subscribes to the topic %s... ", client_id.c_str(), command.c_str());
    if (mqtt.subscribe (command.c_str())) {

      Console.println ("OK");
    }
    else {

      Console.println ("Failed");
    }
  }
}

// Processing events from and to the spa
// return true if events have been processed
bool SpaMqttClientClass::handle () {
  Event event;

  if (!mqtt.connected()) {

    reconnect();
  }
  mqtt.loop();

  while (pullFromSpa (event)) {

    Console.printf ("Spa Event %s received from spa\n", event.toString().c_str());
    if (TypeToMqttTopic.count (event.type()) != 0) {
      String topic, payload;

      topic = settings.topic + "/" + TypeToMqttTopic.at (event.type());
      if (event.isBoolean()) {

        payload = event.value() == true ? "on" : "off";
      }
      else {

        payload = String (event.value());
      }

      if (event.type() == Event::Type::PowerOn && event.value() == true) {
        // force the desired temperature to be published
        pushToSpa (Event (Event::Type::DesiredTemp, 0));
      }
      mqtt.publish (topic.c_str(), payload.c_str());
    }
  }
  return true;
}

// callback function
// called when a message mqtt arrives
void SpaMqttClientClass::mqttCallback (char *topic, byte *payload, unsigned int length)  {
  String p; 
  String t (topic);
  Event event;

  p.concat ((char *) payload, length);
  Console.printf ("MQTT Message arrived in topic: %s=%s\n", topic, p.c_str());
  t.remove (0, t.lastIndexOf ("/") + 1); // remove the prefix to get the command

  event.setType (MqttCommandToType.at (t));
  if (event.isBoolean()) {

    event.setValue (p == "on");
  }
  else {

    event.setValue (p.toInt());
  }
  Console.printf ("\tEvent %s sent to spa\n", event.toString().c_str());
  SpaMqttClient.pushToSpa (event);
}

SpaMqttClientClass SpaMqttClient; // singleton object