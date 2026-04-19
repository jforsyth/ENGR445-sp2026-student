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

/**
 * Necessary includes
 */
#include <stdint.h>
#include <assert.h>

#include "em_usart.h"
#include "em_gpio.h"
#include "em_cmu.h"

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

/**
 * USART Device Handle
 * 
 * Instead of hardcoding "USART1" throughout the code, we create a single
 * pointer variable that points to our USART device. This allows us to:
 * 1. Write generic functions that work with any USART (not just USART1)
 * 2. Later wrap this in a class where the USART can be a class member
 * 3. Make the code easily reusable and interoperable
 * 
 * For now, we hardcode it to USART1, but all USART operations go through
 * this "usart" pointer, making the transition to a class trivial.
 */
USART_TypeDef* usart=USART1;



void delay(int num)
{
  int counter=0;
  for(int i=0;i<num*1000;i++)
    {
      counter++;
    }
}

uint8_t SPI_Transfer(uint8_t data)
{
  //wait for space in the TX buffer
  //use a while loop to poll until transfer buffer is empty
  /**
   * YOUR CODE HERE
   */

  //put data in the TX buffer
  /**
   * YOUR CODE HERE
   */

  //wait for transaction to complete
  //use a while loop to poll that transfer is complete
  /**
   * YOUR CODE HERE
   */

  //pull the resulting transfer from the receive buffer
  /**
   * YOUR CODE HERE
   */

  //return that value from transfer buffer
  return 0x0;
}

void app_init(void)
{



  //configure the required clocks
  //enable the clock for the High-Frequency Peripherals,
  //GPIO, and USART1

  /**
   * YOUR CODE HERE
   */

  //configure the required GPIO
  /**
   * PD0 - MOSI
   * PD1 - MISO
   * PD2 - CLK
   * PD3 - CS
   */
  /**
   * YOUR CODE HERE
   */

  //configure the USART
  /**
   * Setup USART for Synchronous operations
   */
  //set frame size to 8, no parity, no stop..etc.
  /**
   * YOUR CODE HERE
   */

  //setup SYNC bit
  /**
   * YOUR CODE HERE
   */

  //set format to MSB first
  /**
   * YOUR CODE HERE
   */

  /**
   * Configure SPI to 2 MHz
   */

  //calculate desired clock divisor
  /**
   * YOUR CODE HERE
   */

  //Enable SPI MODE 3
  /**
   * YOUR CODE HERE
   */


  //set TX Buffer Interrupt Level to half-full
  /**
   * YOUR CODE HERE
   */


  /**
   * Setup all pins for Route #1
   */
  usart->ROUTE = (0x1) << 8;

  /**
   * Enable all pins in ROUTE[3-0]
   */
  //enable clock
  usart->ROUTE |= (0x1) << 3;
  //enable TXPEN
  usart->ROUTE |= (0x1) << 1;
  //enable RXPEN
  usart->ROUTE |= 0x1;


  //set to master mode
  /**
   * YOUR CODE HERE
   */

  //enable transmit and receive
  /**
   * YOUR CODE HERE
   */


}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  uint8_t randomByte = 0x76;

  uint8_t receivedByte = SPI_Transfer(randomByte);

  bool success=false;
  if (randomByte == receivedByte)
    {
      //done! Woo hoo!
      success=true;
    }
  else
    {
      //failure! Try again.
      success=false;
    }

  while(success)
    {
      // do nothing... you're done!
    }
}
