/**
 * @file lidarFunctions.h
 * @author Kelly Lynch
 * @date 18 Mar 2018
 * @brief Header file for LIDAR control functions
 * @todo Implement
 */

#ifndef H_lidarFunctions
#define H_lidarFunctions

/* -- Includes -- */
#include <Arduino.h>
#include "pins.h"
#include <Wire.h>
#include <LIDARLite.h>

/* -- Function Prototypes -- */
void setupLidar(LIDARLite);
void readLidar(void);

#endif

