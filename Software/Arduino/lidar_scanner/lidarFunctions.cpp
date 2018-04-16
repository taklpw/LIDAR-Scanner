/**
 * @file lidarFunctions.cpp
 * @author Kelly Lynch
 * @date 18 Mar 2018
 * @brief Contains functions for LIDAR-LITE-V3 control and sensing
 * @todo Implement
 */
 
/* -- Includes -- */
#include <Arduino.h>
#include "lidarFunctions.h"
#include "pins.h"

/* -- Functions -- */
/**
* @brief Setup LIDAR Function
* Sets up and configures the LIDAR Lite V3 default mode using
* i2c at 400kHz
* 
* @return Does not return
*/
void setupLidar(LIDARLite lidarObject){
  lidarObject.begin(0, true);
  lidarObject.configure(0);
}


/**
* @brief Read LIDAR Function
* Reads the distance and intensity of the LIDAR-Lite-V3
* 
* @return Does not return
* @TODO Implement
*/
void readLidar(){
  
}
