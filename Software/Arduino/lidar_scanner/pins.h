/**
 * @file pins.h
 * @author Kelly Lynch
 * @date 18 Mar 2018
 * @brief Defines names and types pins
 */

#ifndef H_pins
#define H_pins

/* -- Pin Definitions -- */
/* UART Pins */
extern uint8_t UART_TX_PIN;
extern uint8_t UART_TX_PORTD;
extern uint8_t UART_RX_PIN;
extern uint8_t UART_RX_PORTD;

/* Encoder Pins */
extern uint8_t INDEX_PIN;
extern uint8_t INDEX_PORTD;
extern uint8_t ENCODERA_PIN;
extern uint8_t ENCODERA_PORTD;
extern uint8_t ENCODERB_PIN;
extern uint8_t ENCODERB_PORTD;

/* Motor Pins */
extern uint8_t MOTOR_PWM_PIN;
extern uint8_t MOTOR_PWM_PORTB;
extern uint8_t MOTOR_IN1_PIN;
extern uint8_t MOTOR_IN1_PORTB;
extern uint8_t MOTOR_IN2_PIN;
extern uint8_t MOTOR_IN2_PORTB;

/* I2C Pins */
extern uint8_t SCL_PIN;
extern uint8_t SCL_PORTC;
extern uint8_t SDA_PIN;
extern uint8_t SDA_PORTC;

#endif

