//
// SpaIot Server Alexa Client Example
//
// This example shows how to command a spa using Alexa.
// It use SinricPro library to connect to Alexa.

#include <SpaIot.h>
#include "MyBoardSettings.h"
#include "SpaAlexaClient.h"

using namespace SpaIot;
const unsigned long SerialBaudrate = 115200;

#define WIFI_SSID         "YOUR_WIFI_SSID"    
#define WIFI_PASS         "YOUR_WIFI_PASSWORD"
#define APP_KEY           "YOUR_APP_KEY_HERE"      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET        "YOUR_APP_SECRET_HERE"   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
#define THERMOSTAT_ID     "YOUR_DEVICE_ID_HERE"    // Should look like "5dc1564130xxxxxxxxxxxxxx"

// The Alexa Settings object
AlexaSettings MyAlexaSettings (APP_KEY, APP_SECRET, THERMOSTAT_ID);

// My Spa Server settings
// The SpaModel object is defined in MyBoardSettings.h
const ServerSettings MyServerSettings (SpaModel);

// Create the spa server
SpaServer server;

void setup() {

  // Set software serial baud to SerialBaudrate;
  Console.begin (SerialBaudrate);
  while (!Console) {

    delay (10); // wait user to open serial console
  }

  // Connecting to a WiFi network
  #if defined(ESP8266)
  WiFi.setSleepMode (WIFI_NONE_SLEEP);
  WiFi.setAutoReconnect (true);
  #elif defined(ESP32)
  WiFi.setSleep (false);
  WiFi.setAutoReconnect (true);
  #endif

  WiFi.begin (WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Console.printf (".");
    delay (250);
  }
  Console.printf ("Connected to the Wi-Fi network, IP: %s\n", WiFi.localIP().toString().c_str());

  // Add alexa client to server
  server.addClient (Alexa);

  // start the Alexa client
  Alexa.begin (MyAlexaSettings);

  // then begin the server
  if (server.begin (MyServerSettings)) {

    Console.println ("Server begin succeeded");
  }
  else {

    Console.println ("Server begin failed");
    exit (EXIT_FAILURE);
  }
}

void loop() {

  // All the processing is done in the handle server method
  server.handle();
}
