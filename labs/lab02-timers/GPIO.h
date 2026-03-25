/*
 * GPIO.h
 *
 *  Created on: Jun 7, 2017
 *      Author: Jason Forsyth
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_

#include <em_gpio.h>

#include "Helper.h"

//General GPIO modes and masks
#define PIN_MODE_INPUT 0x1u
#define PIN_MODE_PUSH_PULL 0x4u

#define PIN_MODE_MASK_BITS 0xfu
#define EINT_MASK 0x7u

/**
 * Set pin output value. Only works if correct Port address is provided
 */
void setPinOutputValue(GPIO_Port_TypeDef port, uint32_t pinNum, uint32_t value)
{
  GPIO->P[port].DOUT=(GPIO->P[port].DOUT&~(0x1<<pinNum))|(value&0x1)<<(pinNum);

  return;
}

/**
 * Set pin mode. Only works if correct Port Mode register address is provided
 */
void setPinMode(GPIO_Port_TypeDef port, uint32_t pinNum, uint32_t mode)
{
  if(pinNum>7)
  {
    pinNum%=8;
    GPIO->P[port].MODEH=(GPIO->P[port].MODEH&~(PIN_MODE_MASK_BITS<<(pinNum<<2)))|(mode<<(pinNum<<2));
  }
  else
  {
    GPIO->P[port].MODEL=(GPIO->P[port].MODEL&~(PIN_MODE_MASK_BITS<<(pinNum<<2)))|(mode<<(pinNum<<2));
  }
}

inline void toggleLED(GPIO_Port_TypeDef port, uint32_t pinNum)
{
  writeBit(&GPIO->P[port].DOUTTGL, pinNum, 0x1);
}

#endif /* SRC_GPIO_H_ */
