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
    * LED0 <-> PE2
    * LED1 <-> PE3
    */

   /**
    * Task 1: Easiest method using the EMLIB calls
    * Read the Application Note 12 involving GPIO and
    * use the built in calls to turn on LED1/PE3
    *
    *Application Note on GPIO: https://www.silabs.com/documents/public/application-notes/AN0012.pdf
    * Documentation for GPIO Lib: https://siliconlabs.github.io/Gecko_SDK_Doc/efm32gg/html/group__GPIO.html
    */

    /**
     * Your code below. Use the GPIO_PinModeSet() function to set PE3 as push/pull (output) and HIGH.
     */
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

}
