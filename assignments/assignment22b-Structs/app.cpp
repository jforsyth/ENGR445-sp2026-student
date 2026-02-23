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

#include "em_gpio.h"

void app_init(void)
{
  /**
   * Task 2: Typical Method using the GPIO data structures
   * Read application Note 12 and determine that Port E is Enumeration 4 in the
   * GPIO->P arrays. Use this to configure LED0/PE2 as an output.
   */

  //Use the GPIO structure to READ, MODIFY, and then WRITE the MODEL register
  //associated with PE2. Set the appropriate bits to push/pull. The first and final step
  //is done for you

  //READ the current state of PE MODEL
  uint32_t value = GPIO->P[4].MODEL;

  //MODIFY the data stored in "value" to appropriately set the desired bits
  //reference to the GPIO chapter in the Reference Manual. A dummy line of code has
  //been written for you.
  value = value;


  //WRITE back your modified values into PE MODEL register
  GPIO->P[4].MODEL = value;


  //Use the GPIO structure to read, modify, and then write the DOUT register
  //associated with PE2. Set the appropriate bits to HIGH. Follow the READ, MODIFY, and WRITE
  //cycle as for the previous operation.


  //YOUR CODE BELOW
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
