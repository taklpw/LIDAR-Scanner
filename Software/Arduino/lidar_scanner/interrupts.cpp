/**
 * @file interrupts.cpp
 * @author Kelly Lynch
 * @date 25 Mar 2018
 * @brief Interrupt Functions
 * @todo Implement
 */
 
/* -- Includes -- */
#include "interrupts.h"

 /* -- Interrupt Variables -- */
volatile uint16_t encoderPosition;
volatile bool indexFlag = false;

/* -- Interrupt Functions -- */
/**
* @brief Encoder Index Fall Function
* When the optical element detects a falling edge, this interrupt is
* triggered to set the encoder position to zero and setting a one-time
* index flag.
* 
* @return Does not return
*/
void indexFall(){
  encoderPosition = 0;
  if(!indexFlag){
    indexFlag = true;  
  }
}


/**
* @brief Encoder Counter Function
* When the encoder quadrature A output changes, this function records 
* the direction of the change and either increments or decrements a 
* counter which shows the position of the motor.
* 
* @return Does not return
*/
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

