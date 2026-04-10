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

#include <em_cmu.h>
#include <em_gpio.h>
#include <em_usart.h>

#include "GPIO.h"
#include "Helper.h"

#define EIGHT_DATA_BITS 0x5
#define ONE_STOP_BIT 0x1
#define STOP_BIT_OFFSET 12

#define CLOCK_RATE  14000000ul
#define BAUD_RATE 9600ul
#define OVER_SAMPLE 16ul


void app_init(void)
{
  CMU_ClockEnable(cmuClock_HFPER, true);

  CMU_ClockEnable(cmuClock_GPIO, true);

  CMU_ClockEnable(cmuClock_USART1, true);

  ///////////////////BEGIN USART1 CONFIGURATION///////////////////

  /**
   * Begin configuration of USART1
   * Set for 9600 8N1
   */



  // TODO: Double check that USART frequency is 48MHz with CMU_ClockFreqGet
  int USART_CLOCK = CMU_ClockFreqGet(cmuClock_USART1);

  /**
   * Calculate required DIV value for UART
   * baud rate generator
   * DV = 4*(clock/(br*ovs)-1)
   *
   * Assuming a clock freq of 14MHz and Oversample of 16
   * Desired Baud rate is 9600
   * DIV = 4*(clock/(br*ovs)-1) = 360 = 0x168
   */
  // TODO: Calculate DIV value and set USART1->CLKDIV

  // TODO: Configure FRAME register for the number of data bits

  // TODO: Configure FRAME register for the number of stop bits

  // TODO: Configure FRAME register for the number of parity bits

  //Use ROUTE register to select Location #1 (PD0 and PD1)
  USART1->ROUTE |= (0x1) << 8;

  //Enable TX Pin in ROUTE register
  USART1->ROUTE |= (0x1) << 1;

  //Enable RX Pin in ROUTE register
  USART1->ROUTE |= (0x1);

  // TODO: Enable TXEN and RXEN in CMD

  // TODO: Enable loopback mode in USART1 so TX is internally connected to RX.
  // This eliminates the need for a physical loopback wire between PD0 and PD1.
  // Hint: Set the LOOPBK bit in the USART1->CTRL register.

  ///////////////////END USART1 CONFIGURATION///////////////////

  ///////////////////BEGIN GPIO CONFIGURATION///////////////////

  // TODO: Configure the TX Pin (PD0) on Location #1 as a push-pull output

  // TODO: Configure the RX Pin (PD1) on Location #1 as a push-pull input

  ///////////////////END GPIO CONFIGURATION///////////////////

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  ///////////////////BEGIN USART LOOP BACK///////////////////

  /**
   * Task 1: Transmit a single byte
   */

  // TODO: Create a uint8_t variable and set it equal to some number of your choice
  uint8_t txNum = 0xA;

  // TODO: Write your variable into the TXDATA register of the USART

  // TODO: Poll on the RXDATAV bit in STATUS (indicates there's data in the buffer)
  //       Wait for data to be received

  bool dataAvailable = false;
  while(!dataAvailable)
    {
      //Poll on the RXDATAV bit in STATUS (indicates there's data in the buffer)
    }


  // TODO: Read data out of RXDATA. Should read the same value as your "number" above
  uint8_t rxNum = 0xB;
  // your code here....

  // TODO: Check if the received value matches the transmitted value
  bool task1Success=false;
  if(rxNum==txNum)
    {
      task1Success=true;
    }

  /**
   * Task 2: Transmit multiple bytes. Write to TXDATA twice in a row
   * to send two packets. You should receive two bytes as well.
   */

  // TODO: Create a uint8_t array with two elements. Set each element to an arbitrary number
  uint8_t array[] = {0x01,0x02};

  // TODO: Write your first variable into the TXDATA register of the USART

  // TODO: Write the second variable into TXDOUBLE but left shifted by 8 bits

  // TODO: Poll on the RXFULL bit in STATUS (indicates the FIFO is full)
  //       Wait for data to be received
  bool bufferFull = false;
  while(!bufferFull)
    {
      //Poll on the RXFULL bit in STATUS (indicates the FIFO is full)
    }

  // TODO: Read data out of RX FIFO. Read once from RXDATA and then from RXDOUBLE
  uint8_t value1=0xa;
  uint8_t value2=0xb;
  // your code here....

  bool task2Success=false;
  if(value1==array[0] && value2==array[1])
    {
      task2Success=true;
    }

  if(task1Success==true && task2Success==true)
    {
      //then you've completed the lab. Do nothing forever
      /**
       * Leave this code below to give the processor "something" to do.
       * If there's an empty loop is may optimize away lines of code from above.
       * Just keep it here for a sanity check.
       */
      int counter=0;
      while(1)
        {
          counter++;
        }

    }
}
