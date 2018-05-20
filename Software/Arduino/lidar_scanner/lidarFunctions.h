/**
 * @file lidarFunctions.h
 * @author Kelly Lynch
 * @date 20 May 2018
 * @brief Header file for LIDAR control functions
 */

#ifndef H_lidarFunctions
#define H_lidarFunctions

/* -- Includes -- */
#include <Arduino.h>
#include "pins.h"
#include <Wire.h>
#include <LIDARLite.h>

/* -- Function Prototypes -- */
void setupLidar(LIDARLite&);
String readLidar(LIDARLite&);

#endif

