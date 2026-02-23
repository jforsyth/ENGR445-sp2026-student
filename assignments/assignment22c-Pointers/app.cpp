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

#include <em_gpio.h>

//add UL to make sure the math is done correctly
#define GPIO_BASE_ADDRESS 0x40006000UL

//Port E definitions and addresses
#define GPIO_PE_MODEL_OFFSET 0x094UL
#define GPIO_PE_DOUT_OFFSET 0x09CUL
#define GPIO_PE_MODEL_ADDR (GPIO_BASE_ADDRESS+GPIO_PE_MODEL_OFFSET)
//#define GPIO_PE_DOUT_ADDR //implement if you desire

//General GPIO modes and masks
#define PIN_MODE_INPUT 0x1u
#define PIN_MODE_PUSH_PULL 0x4u
#define PIN_MODE_MASK_BITS 0xfu

void app_init(void)
{

  /**
   * Task 3: Pure Pointer Math. Calculate the addresses for PE_MODEL
   * and PE_DOUT to manually configure the bits for LED1/PE3
   */


  //READ the PE3 MODEL Register
  volatile uint32_t* PE_MODEL_PTR = (uint32_t*)GPIO_PE_MODEL_ADDR;

  //your code here

  // MODIFY and WRITE the PE3 MODEL Register
  //set PE3 mode to Push/Pull Output

  //your code here

  //READ the PE3 DOUT Register

  //your code here


  //MODIFY and WRITE the PE3 DOUT Register
  //set PE3 DOUT to HIGH

  //your code here
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
