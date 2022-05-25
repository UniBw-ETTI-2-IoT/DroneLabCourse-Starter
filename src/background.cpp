/*
 * background.cpp
 *
 * Contains all the other stuff that's not really relevant in an IoT Context
 * and therefore shouldn't be of interest for the students.
 *
 * UniBw ETTI IoT Tello Lab Course
 *  inspired by examples of https://github.com/RoboMaster/RMTT_Libs and 
 *  https://github.com/Suxsem/pubsubclient both libraries may be edited 
 *  or have changed by now, don't change the ones provided with this project!
 * author: Karl Scholz, WIWeB GF250
 * last change: 2022 05 18
 */

#include "background.h"

RMTT_RGB tt_rgb;
RMTT_Matrix tt_matrix;
uint8_t tof_display[128] = {0}; // LED Matrix
bool bAircraftArmed = false;

const char *arszFlightStateNames[6] = {
    "IdleOnM1",
    "M1toM2",
    "M2toWall",
    "AroundWalltoM3",
    "M3toM4",
    "M4toM1Land"};

void BackgroundSetupBEFORE()
{
  Serial.begin(115200, SERIAL_8N1); // UART to tello
  Wire.begin(27, 26);               // front LIDAR
}
void BackgroundSetupAFTER()
{
  ConsolePrint("\nInitializing UART Connection to Tello...\npress the left button when you see a green light!\n"); // UART
  Serial1.begin(1000000, SERIAL_8N1, 23, 18);
  tt_sdk.startUntilControl();

  tt_rgb.Init(); // top LED
  led_effect_init();
  led_effect_breath(0, 255, 0, 0.25);

  Wire.setClock(100000); // front Lidar
  tt_lidar.SetTimeout(500);
  while (!tt_lidar.Init())
  {
    ConsolePrint("Failed to detect and initialize lidar sensor!\n");
  }

  tt_matrix.Init(127); // Matrix
  tt_matrix.SetLEDStatus(RMTT_MATRIX_CS, RMTT_MATRIX_SW, RMTT_MATRIX_LED_ON);
  matrix_effect_init(255);
  matrix_effect_move_str("IdleM1 ", 7, 'b', 'l', 2.5);
}

void BackgroundLoop()
{
  // checks MQTT connection and reestablishes it as well as WiFi if failed
  if (!mqtt_client.connected())
  {
    Reconnect();
  }

  // Publishes "heartbeat", alternating numbers from 0 to 10 every 5 seconds
  // ...so you can implement or be kind of a watchdog yourself over mqtt ;)
  long lNow = millis();
  static long lLastMsgTime = 0;
  if (abs(lNow - lLastMsgTime) > 5000)
  {
    lLastMsgTime = lNow;
    static float fValue = 0;
    static float fIncrement = +.25;
    fIncrement = (fValue >= 1) ? (-.25) : ((fValue <= 0) ? (+.25) : (fIncrement));
    fValue += fIncrement;
    char szText[8];
    dtostrf(fValue, 1, 2, szText);
    mqtt_client.publish((String(szAircraftID) + "/heartbeat").c_str(), szText, 0, false);
    
    // shows Flightstate in MQTT
    mqtt_client.publish((String(szAircraftID) + "/flightstate").c_str(), String(arszFlightStateNames[tFlightState]).c_str(),0, false);
  }

  // makes sure the LED is green when the Aircraft is safe and red when operating
  static bool bCurrentLEDArmed;
  if (bCurrentLEDArmed != bAircraftArmed)
  {
    bCurrentLEDArmed = bAircraftArmed;
    if (bAircraftArmed)
    {
      led_effect_breath(255, 0, 0, 2);
    }
    else
    {
      led_effect_breath(0, 255, 0, 0.25);
    }
  }
  

  switch (tFlightState)
  {
  case IdleOnM1:
    static bool bFirstTimeIdleOnM1 = true;
    if (bFirstTimeIdleOnM1)
    {
      matrix_effect_move_str("IdleM1 ", 7, 'b', 'l', 2.5);
      bFirstTimeIdleOnM1 = false;
    }
    if (strCommand == "Cleared for Takeoff")
    {
      matrix_effect_move_str("Takeoff ", 8, 'r', 'l', 2.5);
      led_effect_breath(255, 0, 0, 2);
      bAircraftArmed = true;
      bFirstTimeIdleOnM1 = true;
    }
    break;

  case M1toM2:
    matrix_effect_move_str("1->2 ", 5, 'b', 'l', 2.5);
    break;

  case M2toWall:
    break;

  case AroundWalltoM3:
    matrix_effect_init(255);
    matrix_effect_move_str("Wall->3 ", 8, 'p', 'l', 2.5);
    break;

  case M3toM4:
    matrix_effect_move_str("3->4 ", 5, 'b', 'l', 2.5);
    break;

  case M4toM1Land:
    static bool bFirstTimeM4toM1Land = true;
    if (bFirstTimeM4toM1Land)
    {
      matrix_effect_move_str("Holding ", 8, 'b', 'l', 2.5);
      bFirstTimeM4toM1Land = false;
    }
    if (strCommand == "Cleared to Land")
    {
      matrix_effect_move_str("4->1 Land ", 10, 'b', 'l', 2.5);
      bAircraftArmed = false;
      bFirstTimeM4toM1Land = true;
    }
    tt_sdk.sendTelloCtrlMsg("back 20"); //send another message that doesn't do really much to tello but it keeps its watchdog is happy and doesn't land it automatically after receiving 20x the user same message (unvalid msgs get ignored) or staying to long in the same location idk with "back 20" it flew till the batt died.
    break;

  default:
    ConsolePrint("Fatal OSI Layer 8 Error!\n");
  }
}

void SetupWifi()
{
  delay(100);
  ConsolePrint("\nTrying to connect to " + String(szSSID) + "\n");
  WiFi.setHostname(szAircraftID);
  WiFi.begin(szSSID, szPassword);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    ConsolePrint(".");
  }
  ConsolePrint("\nWiFi connected successfully to " + String(szSSID) + "\nIP adress: ");
  ConsolePrintln(WiFi.localIP());
}

void Reconnect()
{
  while (!mqtt_client.connected())
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      SetupWifi();
    }
    delay(10);
    ConsolePrint("Attempting MQTT connection...");
    if (mqtt_client.connect(szAircraftID))
    {
      ConsolePrint("connected\n");
      mqtt_client.publish((String(szAircraftID) + "/command").c_str(), "I'm listening to commands on this topic. :)", 0, false);
      mqtt_client.subscribe((String(szAircraftID) + "/command").c_str(), 1);
    }
    else
    {
      ConsolePrint("failed, rc=" + String(mqtt_client.state()) + " try again in 5 seconds\n");
      delay(5000);
    }
  }
}

void MQTTprint(String strData)
{
  mqtt_client.publish((String(szAircraftID) + "/console").c_str(), strData.c_str(), 0, false);
}

void DisplayLidarOnMatrix(float fRange)
{
  matrix_effect_deinit();
  fRange = (fRange > RANGE_MAX) ? (1.01 * RANGE_MAX) : ((fRange <= 0) ? (1.01 * RANGE_MAX) : (fRange)); // limits max value and filters obvious error 0.00 to max value
  if (tt_lidar.TimeoutOccurred())
  {
    ConsolePrint("LIDAR TIMEOUT\n");
  }
  else
  {
    mqtt_client.publish((String(szAircraftID) + "/lidar[m]").c_str(), String(fRange).c_str(),0, false); // for MQTT Explorer graphical view
    for (int i = 63; i >= 0; i--)
    {
      if (fRange / RANGE_MAX > (float)i / (float)63)
      {                                                                    // Gradient from Red (Close) to Blue (far)
        tof_display[i * 2 + 1] = 255 - (int)(((float)i / 63.0f) * 255.0f); // blue
        tof_display[i * 2] = (int)(((float)i / 63.0f) * 255.0f);           // red
      }
      else
      {
        // Switch all other LEDS off
        tof_display[i * 2 + 1] = 0;
        tof_display[i * 2] = 0;
      }
    }
    tt_matrix.SetAllPWM(tof_display); // Send Data
    delay(15);
  }
}