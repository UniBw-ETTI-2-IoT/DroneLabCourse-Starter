/*
 * main.cpp
 *
 * UniBw ETTI IoT Tello Lab Course
 * author: Karl Scholz, WIWeB GF250
 * last change: 2022 05 18
 */

#include "main.h"

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

RMTT_Protocol tt_sdk;
RMTT_TOF tt_lidar;


const char *szAircraftID;
const char *szPartnerAircraftID;

const char *szSSID;
const char *szPassword;

const char *szMQTTBroker;
const uint16_t uiMQTTPort = -1;

FLIGHTSTATES_T tFlightState;
String strCommand = "";

void setup()
{
  BackgroundSetupBEFORE();
  // WiFi

  // MQTT

}

void loop()
{
  // MQTT Loop
  
  // BackgroundLoop
  BackgroundLoop();
  // State Machine
  switch (tFlightState)
  {
  case IdleOnM1:
    break;

  case M1toM2:
    break;

  case M2toWall:
    break;

  case AroundWalltoM3:
    break;

  case M3toM4:
    break;

  case M4toM1Land:
    break;

  default:
    break;
  }
}

void Callback(char *szTopic, byte *yMessage, unsigned int uiLength)
{

}