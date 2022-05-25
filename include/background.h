/*
 * background.hpp
 *
 * Contains all the other stuff that's not really relevant in an IoT Context
 * and therefore shouldn't be of interest for the students.
 *
 * UniBw ETTI IoT Tello Lab Course
 *  inspired by examples of https://github.com/RoboMaster/RMTT_Libs and 
 *  https://github.com/Suxsem/pubsubclient both libraries may be edited 
 *  or have changed by now, don't change the ones provided with this project!
 * author: Karl Scholz, WIWeB GF250
 * last change: 2022 04 03
 */

#ifndef _BACKGROUND_H
#define _BACKGROUND_H

#include "main.h"



/* Takes care of other stuff.
 *
 * Parameters:
 * - Nothing
 * Returns:
 * - Nothing
 *
 * Last Change: KS 2022 03 14
 *                                                                            */
void BackgroundSetupBEFORE();

/* Takes care of other stuff.
 *
 * Parameters:
 * - Nothing
 * Returns:
 * - Nothing
 *
 * Last Change: KS 2022 03 14
 *                                                                            */
void BackgroundSetupAFTER();

/* Takes care of other stuff.
 *
 * Parameters:
 * - Nothing
 * Returns:
 * - Nothing
 *
 * Last Change: KS 2022 04 03
 *                                                                            */
void BackgroundLoop();

/* Reconnects the MQTT connection as well as the WiFi Connection.
 *
 * Parameters:
 * - None
 * Returns:
 * - Nothing
 *
 * Last Change: KS 2022 03 02
 *                                                                            */
void Reconnect();

/* Publishes the given String to <szAircraftID>/console.
 *
 * Parameters:
 * - String strData
 * Returns:
 * - Nothing
 *
 * Last Change: KS 2022 03 02
 *                                                                            */
void MQTTprint(String strData);

/* Initializes a WiFi Connection in Client Mode as specified in main.h.
 *
 * Parameters:
 * - None
 * Returns:
 * - Nothing
 *
 * Last Change: KS 2022 03 02
 *                                                                            */
void SetupWifi();

/* Displays given lidar value as a percentage of RANGE_MAX on the LED Matrix.
 * Also publishes it in a seperate Subtopic to view it graphicly in debugging
 * Programs like MQTT Explorer.
 *
 * Parameters:
 * - float fRange in METERS
 * Returns:
 * - Nothing
 *
 * Last Change: KS 2022 03 14
 *                                                                            */
void DisplayLidarOnMatrix(float fRange);

#endif