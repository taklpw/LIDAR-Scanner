/**
 * @file lidarFunctions.cpp
 * @author Kelly Lynch
 * @date 20 May 2018
 * @brief Contains functions for LIDAR-LITE-V3 control and sensing
 */
 
/* -- Includes -- */
#include <Arduino.h>
#include "lidarFunctions.h"
#include "interrupts.h"
#include "pins.h"

/* -- Functions -- */
/**
* @brief Setup LIDAR Function
* Sets up and configures the LIDAR Lite V3 default mode using
* i2c at 400kHz
* 
* @return Does not return
*/
void setupLidar(LIDARLite &lidarObject){
  lidarObject.begin(0, true);
  lidarObject.configure(0);
}


/**
* @brief Read LIDAR Function
* Reads the distance and intensity of the LIDAR-Lite-V3 and gives a string with the information
* 
* @return String of readings
*/
String readLidar(LIDARLite &lidarObject){
    /* -- Report LIDAR Measurements -- */
  /* Record time */
  unsigned long distReadTime = micros();
  /* Read distance of lidar */
  int lidarDistance = lidarObject.distance();
  /* Read intensity of the return */
  byte arrayToSave[1];
  char intensityReg = 0x0e;
  lidarObject.read(intensityReg, 1, arrayToSave, false, LIDARLITE_ADDR_DEFAULT);
  byte lidarIntensity = arrayToSave[0];
  /* Report Angle of Spinner */
  double angleRads = 2 * fmod(encoderPosition*0.0076968, 1.047198) + (PI/6);

  String lidarString = "~S:";
  lidarString += distReadTime;
  lidarString += ",";
  lidarString += String(angleRads, 6);
  lidarString += ",";
  lidarString += lidarDistance;
  lidarString += ",";
  lidarString += lidarIntensity;
  lidarString += "~~\n\r";
  return lidarString;
}
