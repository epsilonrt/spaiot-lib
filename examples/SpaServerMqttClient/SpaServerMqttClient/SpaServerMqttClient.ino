//
// SpaIot Server MQTT Client Example
//
// This example shows how to connect a spa to a MQTT broker.
// You must have a MQTT broker installed on your network !
// You can use MQTT Explorer to interact with your spa

// Define the SpaMqttClient class and Serial Console
#include "SpaMqttClient.h"
#include "MySpaConfig.h"

const unsigned long SerialBaudrate = 115200;

// WiFi
const char *ssid = "WIFI_SSID"; // Enter your Wi-Fi name
const char *password = "WIFI_PASSWD";  // Enter Wi-Fi password

// Configure the MQTT broker connection (MQTT_LOGIN and MQTT_PASSWD are optional)
MqttSettings settings ("broker.hivemq.com", "MQTT_LOGIN", "MQTT_PASSWD");

// My custom configuration
const HardwareSettings MyConfig (MyBus, SspLeds, MyButtons);
const ServerSettings MyServerSettings ("MyConfig");
SpaServer spa;

WiFiClient espClient;

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
  HardwareSettings::addToRegister (MyServerSettings.spaModel(), MyConfig);
  spa.addClient (SpaMqttClient);

  // client connecting to the mqtt broker
  SpaMqttClient.begin (espClient, settings);

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
