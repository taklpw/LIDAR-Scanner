/**
 * @author Kelly Lynch
 * @date 2018-03-25
 * @brief Arduino code to control a motor, IMU sensor, and LIDAR-Lite-V3.
 *
 * @bugs None known
 * @todo everything
 */

/* -- Includes -- */
#include <EnableInterrupt.h>
#include "pins.h"
#include "interrupts.h"
#include "motorFunctions.h"
#include "imuFunctions.h"
#include "lidarFunctions.h"


void setup() {
  /* Setup Serial Communication */
  Serial.begin(250000);
  
  /* Enable Interrupts */
  enableInterrupt(ENCODERA_PIN, encoderAChange, CHANGE);
  enableInterrupt(INDEX_PIN, indexFall, FALLING); 
  
  /* Setup Peripherals */
  setupMotor();
  setupLidar();
  setupIMU();
  
  /* Seek Motor Index */
  seekIndex();
}


void loop() {  
  Serial.println(encoderPosition);
}
