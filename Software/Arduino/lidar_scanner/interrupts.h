/**
 * @file interrupts.h
 * @author Kelly Lynch
 * @date 20 May 2018
 * @brief Header file for interrupt functions and variables
 */

#ifndef H_interrupts
#define H_interrupts

/* -- Includes -- */
#include <Arduino.h>
#include "pins.h"

 /* -- Interrupt Variables -- */
extern volatile uint16_t encoderPosition;
extern volatile bool indexFlag;
extern volatile uint16_t maxEncoderPosition;

/* -- Interrupt Function Prototypes -- */
void indexFall(void);
void encoderAChange(void);

#endif

