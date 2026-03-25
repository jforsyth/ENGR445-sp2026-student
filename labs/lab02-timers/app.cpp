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

#include <em_cmu.h>
#include <em_gpio.h>
#include <em_timer.h>

#include <stdint.h>
#include <stdbool.h>

#include "GPIO.h"
#include "Timer.h"
#include "Helper.h"

/**
 * TIMER0 interrupt handler.
 * Called on every TIMER0 overflow (each half-second / quarter-second / etc.).
 */
void TIMER0_IRQHandler(void)
{
  // TODO: clear TIMER0 overflow interrupt flag (IFC bit 0).
  // TODO: toggle LED0 (PE2).

}

/**
 * TIMER1 interrupt handler.
 */
void TIMER1_IRQHandler(void)
{

}

/**
 * GPIO Odd Interrupt Handler.
 * Triggered by a falling edge on PB9 (PB0, odd pin).
 * Each press advances TIMER0 to the next blink rate:
 *   HALF_SECOND -> QUARTER_SECOND -> TENTH_SECOND -> (repeat)
 * TODO:
 *   1. Clear all odd GPIO interrupt flags
 *   2. Stop TIMER0
 *   3. Update TIMER0 TOP to the next period from the periods[] cycle array
 *   4. Clear TIMER0 counter and any pending TIMER0 flags
 *   5. Restart TIMER0
 *   6. Increment the cycle counter
 */
void GPIO_ODD_IRQHandler()
{
  //helpful array to hold the timer periods for each button press
    const static uint16_t periods[] = {HALF_SECOND, QUARTER_SECOND, TENTH_SECOND};
    
//helpful counter to keep track of where we are in the cycle of periods
  static int counter = 1;

}

/**
 * GPIO Even Interrupt Handler.
 */
void GPIO_EVEN_IRQHandler()
{
  static bool measuring = false;

}

void app_init(void)
{
  // Enable the clock for the High-Frequency Peripheral bus
  CMU_ClockEnable(cmuClock_HFPER, true);

  // Enable the peripheral clock for TIMER0
  CMU_ClockEnable(cmuClock_TIMER0, true);

  // Enable the peripheral clock for TIMER1
  CMU_ClockEnable(cmuClock_TIMER1, true);

  // Enable the peripheral clock for GPIO
  CMU_ClockEnable(cmuClock_GPIO, true);

  /**
   * TODO: Setup PB9 (PB0) as input with falling-edge external interrupt
    * Hint: configure MODEH, EXTIPSELH, EXTIFALL/EXTIRISE, IFC, IEN, and NVIC.
   */

  /**
   * TODO: Setup PB10 (PB1) (if needed)
   */

  /**
   * TODO: Set PE2 (LED0) and PE3 (LED1) as push-pull outputs
   * Hint: use setPinMode() with PIN_MODE_PUSH_PULL
   */

  /**
   * TODO: Configure TIMER0 for 0.5-second blinking of LED0
   *  Follow guidance in 2.4c interrupts to setup timer
   * and enable interrupts
   */

  /**
   * TODO: Configure TIMER1 (if needed)
   */
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
    /**
     * No code could be placed here.
     */
}
