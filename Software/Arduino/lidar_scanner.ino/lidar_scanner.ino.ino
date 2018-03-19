/**
 * @author Kelly Lynch
 * @date 2018-03-18
 * @brief Arduino code to control a motor, IMU sensor, and LIDAR-Lite-V3.
 *
 * @bugs None known
 * @todo everything
 */

/* -- Includes -- */
#include <EnableInterrupt.h>
#include "pins.h"

/* -- Global Variables -- */


/* -- Interrupt Variables -- */
volatile uint16_t encoderPosition;
//volatile int ;

/* -- Interrupt ISRs -- */
void indexFall(){
  encoderPosition = 0;
}

void encoderAChange(){
  bool encoderAState = bitRead(PORTD, ENCODERA_PORTD);
  bool encoderBState = bitRead(PORTD, ENCODERB_PORTD);
  /* if encoderB is not in the same state as encoder A, position is increasing */
  if(encoderAState != encoderBState){
    encoderPosition--;
  }else{
    encoderPosition++;
  }
}


void setup() {
  /* Setup Motor Related Pins */
  pinMode(ENCODERA_PIN, INPUT);
  pinMode(ENCODERB_PIN, INPUT);
  pinMode(INDEX_PIN, INPUT);
  pinMode(MOTOR_PWM_PIN, OUTPUT);
  pinMode(MOTOR_EN_PIN, OUTPUT);

  /* Setup LIDAR Related Pins */

  /* Setup  IMU Related Pins */

  /* Setup Serial Communication */
  Serial.begin(250000);

  /* Enable Interrupts */
  enableInterrupt(ENCODERA_PIN, encoderAChange, CHANGE);
  enableInterrupt(INDEX_PIN, indexFall, FALLING);
}

void loop() {
  /* Rotate motor until index is found */
  
  /*  */
  
  Serial.println(encoderPosition);
}
