/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

//include GPIO libraries
#include "em_gpio.h"

//add UL to make sure the math is done correctly
#define GPIO_BASE_ADDRESS 0x40006000UL

//Port B definitions and addresses
#define GPIO_PB_MODEH_OFFSET 0x02CUL
#define GPIO_PB_DIN_OFFSET 0x040UL
#define GPIO_PB_MODEH_ADDR (GPIO_BASE_ADDRESS+GPIO_PB_MODEH_OFFSET)
#define GPIO_PB_DIN_ADDR (GPIO_BASE_ADDRESS+GPIO_PB_DIN_OFFSET)

//Port E definitions and addresses
#define GPIO_PE_MODEL_OFFSET 0x094UL
#define GPIO_PE_TOGGLE_OFFSET 0x0A8UL
#define GPIO_PE_DOUT_OFFSET 0x09CUL
#define GPIO_PE_MODEL_ADDR (GPIO_BASE_ADDRESS+GPIO_PE_MODEL_OFFSET)
#define GPIO_PE_TOGGLE_ADDR (GPIO_BASE_ADDRESS+GPIO_PE_TOGGLE_OFFSET)
#define GPIO_PE_DOUT_ADDR (GPIO_BASE_ADDRESS+GPIO_PE_DOUT_OFFSET)

//General GPIO modes and masks
#define PIN_MODE_INPUT 0x1u
#define PIN_MODE_PUSH_PULL 0x4u
#define PIN_MODE_MASK_BITS 0xfu

//helper defines for push button logic
#define HIGH 0x1
#define LOW 0x0

/**
 * Read the value of a single bit from a memory address
 * @param addr pointer to 32-bit memory location
 * @param pos bit-wise position [31-0] of the bit to read
 * @return value of that bit as an unsigned integer
 */
unsigned int readBit(volatile const uint32_t *addr, unsigned int pos)
{
  return (*addr>>pos) & 0x1;
}

/**
 * Write a single bit into a memory location
 * @param addr pointer to a 32-bit memory location
 * @param bitPos the bit-wise position [31-0] of the bit to be written
 * @param val the value to be written into the bit position
 * @return void
 */
void writeBit(volatile uint32_t *addr, unsigned int bitPos, unsigned int val)
{
  uint32_t tmp = *addr;
  *addr = (tmp & ~(1 << bitPos)) | ((val & 0x1) << bitPos);
  return;
}



void app_init(void)
{
  //set LED0/PE2 and LED1/PE3 to be outputs
  /**
   * Your code here
   */

  //clear both LEDs (set their DOUT value to 0x0)
  /**
   * Your code here
   */

  //set Button0/PB9 and Button1/PB10 to be inputs
  /**
   * Your code here
   */
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{


  /**
   * TASK 1: While PB0 is held down, LED0 should illuminate. Otherwise LED0 should be off.
   */

  /**
   * Read the value of PB0. Based upon the value read in, illuminate (or not) LED0.
   */




  /**
   * TASK 2: Based upon the behavior of PB1, toggle LED1. Effectively, change the state of the
   * pin as it changes from LOW (pressed) to HIGH (released).
   */

  /**
   * Use these two variables to track the state of PB1. As static variables they will
   * initialize to the value HIGH but remember any future values assigned to them
   */
  static unsigned int prevButtonState = HIGH;
  static unsigned int currentButtonState = HIGH;

  /**
   * Read current state of Button0/PB10. Store the value in currentButtonState.
   */

  /**
   * Check *IF* the button has just transitions from LOW to HIGH. If so, toggle the LED between
   * ON and OFF. You may want to use the appropriate TOGGLE register built into the GPIO
   */


  /**
   * Keep this  line of code so you can remember the "old" value of PB1
   */
  prevButtonState = currentButtonState;
}
