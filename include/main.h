/*
 * main.h
 *
 * UniBw ETTI IoT Tello Lab Course
 * author: Karl Scholz, WIWeB GF250
 * last change: 2022 04 26
 */

#ifndef _MAIN_H
#define _MAIN_H

// table of contents:
// precompiler command defines///////////////////////////////////////////////////
// includes//////////////////////////////////////////////////////////////////////
// defines///////////////////////////////////////////////////////////////////////
// macros////////////////////////////////////////////////////////////////////////
// user typedefs/////////////////////////////////////////////////////////////////
// extern variables//////////////////////////////////////////////////////////////
// function prototypes///////////////////////////////////////////////////////////

// precompiler command defines///////////////////////////////////////////////////
#define DEBUG_CONSOLE // activates printing to Serial as well publishing it to MQTT AircraftID/console via the ConsolePrint(x) macro

// includes//////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include <RMTT_Libs.h>
#include <Wire.h>

#include "background.h"

// defines///////////////////////////////////////////////////////////////////////
#define RANGE_MAX (0.64) // Max Lidar Range is set to 0.64 m so 1 box on matrix equals to 1cm
#define HEIGHTAGL "80" //in cm keep the quotes cause it stays a const char after conversion like that

// macros////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_CONSOLE
#define ConsolePrint(x) \
    Serial.print(x);    \
    MQTTprint(String(x));
#define ConsolePrintln(x) \
    Serial.println(x);    \
    MQTTprint(String(x) + "\n");
#endif
#ifndef DEBUG_CONSOLE
#define ConsolePrint(x) ;
#define ConsolePrintln(x) ;
#endif

// user typedefs/////////////////////////////////////////////////////////////////
typedef enum
{
    IdleOnM1,
    M1toM2,
    M2toWall,
    AroundWalltoM3,
    M3toM4,
    M4toM1Land
} FLIGHTSTATES_T;

// extern variables//////////////////////////////////////////////////////////////
extern WiFiClient wifi_client;
extern PubSubClient mqtt_client;

extern RMTT_Protocol tt_sdk;
extern RMTT_RGB tt_rgb;
extern RMTT_TOF tt_lidar;
extern RMTT_Matrix tt_matrix;

extern const char *szAircraftID;
extern const char *szPartnerAircraftID;

extern const char *szSSID;
extern const char *szPassword;

extern const char *szMQTTBroker;
extern const uint16_t uiMQTTPort;

extern uint8_t tof_display[128]; // LED Matrix

extern bool bAircraftArmed;
extern FLIGHTSTATES_T tFlightState;
extern String strCommand;

// function prototypes///////////////////////////////////////////////////////////

/* Setup Function of the Arduino Framework. Will be the entrance point in your
 * program. Similar to the int main(void) Function in a classic C program.
 *
 * Parameters:
 * - Nothing
 * Returns:
 * - Nothing
 *
 * Last Change: KS 2022 03 02
 *                                                                            */
void setup();

/* Loop Function of the Arduino Framework. Will be run after setup() repeatedly
 * automatically. Similar to the while(1) structure in a traditional superloop.
 *
 * Parameters:
 * - Nothing
 * Returns:
 * - Nothing
 *
 * Last Change: KS 2022 04 03
 *                                                                            */
void loop();

/* Callback Function that gets passed to the MQTT Library, it'll be run whenever
 * there is something new on a topic you subscribed to.
 *
 * Parameters:
 * - char* szTopic
 * - byte* yMessage
 * - unsigned int uLength
 * Returns:
 * - Nothing
 *
 * Last Change: KS 2022 03 31
 *                                                                            */
void Callback(char *szTopic, byte *yMessage, unsigned int uLength);



#endif