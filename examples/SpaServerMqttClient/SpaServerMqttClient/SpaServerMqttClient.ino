//
// SpaIot Server MQTT Client Example
//
// This example shows how to connect a spa to a MQTT broker.
// You must have a MQTT broker installed on your network !
// You can use MQTT Explorer to interact with your spa
// This program publishes events from the spa to the MQTT broker in the topic "epsilonrt/spaiot" :
// - power : on/off
// - filter : on/off
// - bubble : on/off
// - heater : on/off
// - heat_reached : on/off
// - desired_temp : temperature
// - water_temp : temperature
// and subscribes to the topic "epsilonrt/spaiot" to receive commands from the MQTT broker :
// - command/power : on/off
// - command/filter : on/off
// - command/bubble : on/off
// - command/heater : on/off
// - command/desired_temp : temperature

#include <SpaIot.h>
#include "MyBoardSettings.h"
#include "SpaMqttClient.h"

using namespace SpaIot;
const unsigned long SerialBaudrate = 115200;

// WiFi
const char *ssid = "WIFI_SSID"; // Enter your Wi-Fi name
const char *password = "WIFI_PASSWD";  // Enter Wi-Fi password

// Configure the MQTT broker connection, enter your MQTT broker IP address or DNS name, login and password, and the topic prefix
MqttSettings settings ("MQTT_URL", "MQTT_LOGIN", "MQTT_PASSWD", "epsilonrt/spaiot");

// My Spa Server settings
const ServerSettings MyServerSettings (SpaModel);

// The WiFi client used to connect to the MQTT broker
WiFiClient wifiClient;

// Create the spa server
SpaServer spa;

void setup() {

  // Set software serial baud to SerialBaudrate;
  Console.begin (SerialBaudrate);
  while (!Console) {

    delay (10); // wait user to open serial console
  }

  // Connecting to a WiFi network
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED) {

    delay (500);
    Console.println ("Connecting to WiFi...");
  }
  Console.printf ("Connected to the Wi-Fi network, IP: %s\n", WiFi.localIP().toString().c_str());

  // Adding the spa config to the register, then adding the client to the spa server
  spa.addClient (SpaMqttClient);

  // client connecting to the mqtt broker
  SpaMqttClient.begin (wifiClient, settings);

  // then begin the server
  if (spa.begin (MyServerSettings)) {

    Console.println ("Server begin succeeded");
  }
  else {

    Console.println ("Server begin failed");
    exit (EXIT_FAILURE);
  }
}

void loop() {

  spa.handle();
}
