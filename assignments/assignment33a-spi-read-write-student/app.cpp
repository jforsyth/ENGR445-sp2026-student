#include <em_device.h>
#include <em_chip.h>
#include <em_cmu.h>
#include <em_emu.h>
#include <em_gpio.h>
#include <em_timer.h>
#include <em_usart.h>

#include <stdint.h>
#include <stdbool.h>


/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

/**
 * Handle to my USART. Enables to be used with any USART
 * in the system. Will be set to USART1 for now. But will be a
 * place holder for future expansion
 */
USART_TypeDef* usart = USART1;

void delay(int num)
{
  int counter=0;
  for(int i=0;i<num*1000;i++)
    {
      counter++;
    }
}

void setup_clocks()
{
  //enable the clock for the High-Frequency Peripherals
  CMU_ClockEnable(cmuClock_HFPER, true);

  //enable GPIO clock
  CMU_ClockEnable(cmuClock_GPIO, true);

  //enable the USART1 clock
  CMU_ClockEnable(cmuClock_USART1, true);
}

void setup_gpio_pins()
{
  /**
   * PD0 - MOSI - SDA
   * PD1 - MISO - SDO
   * PD2 - CLK - SCLK
   * PD3 - CS
   */


  /**
   * Your code here
   */
}

void setup_SPI()
{
  /**
   * Setup USART for Synchronous operations
   */

  /**
   * Configure the size of the data packet to be sent.
   * It should be 8-bits with no stop, or parity bits.
   * Enable synchronous operation and Most Significant Bit First operation.
   */

  /**
   * Your code here
   */


  /**
   * Setup all pins for Route #1
   * Dr. Forsyth has completed this.
   */
  usart->ROUTE = (0x1) << 8;

  //re-route clock pin, set CLKPEN[3]
  usart->ROUTE |= (0x1) << 3;

  //re-route TX pin, set TXPEN[1]
  usart->ROUTE |= (0x1) << 1;

  //re-route RX pin, set RXPEN[0]
  usart->ROUTE |= 0x1;


}

/**
 * Conduct a single 8-bit transfer on the SPI interface
 * @param data Bits to be sent on SPI interface
 * @return Bits returned from transaction
 */
uint8_t SPI_Transfer(uint8_t data)
{

  /**
   * Your code here
   */

  return 0x0;
}

/**
 * Read from an internal register of the ADXL 345.
 * @param address Internal ADXL345 register address to read
 * @return Result of read operation
 */
uint8_t readRegister(uint8_t address)
{

  /**
   * Your code here
   */

  return 0x0;

}

/**
 * Write to an internal register of the ADXL345
 * @param address Internal register to be written to
 * @param data Bits that will be written to the register
 */
void writeRegister(uint8_t address, uint8_t data)
{
  /**
   * Your code here
   */

  //return nothing
  return;


}

void app_init(void)
{

  //configure the required clocks
  setup_clocks();

  //configure the required GPIO
  setup_gpio_pins();

  //configure the USART
  setup_SPI();


}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

  /**
   * Task 1: Complete the SPI Transfer and the Read Register functions. Once done
   * the following lines will attempt to read from the DEVID register at
   * memory address 0x0 in the ADXL. The result should be 0xE5 if performed
   * correctly.
   */

  uint8_t register_to_read = 0x0;
  uint8_t result = readRegister(register_to_read);

  if(result==0xE5){
      //success!!!
      int success=0;
  }
  else{
      int failure=0;
  }

  /**
   * Task 2: Complete Write Register. Write a random set of data to a register
   * in the ADXL345 that will not be used (TAP_THRESH?). Read that data back, if the
   * same result is returned, then the Write function is correct.
   */

  uint8_t data_to_write = 0x1F;
  uint8_t register_to_write = 0x1D;

  writeRegister(register_to_write, data_to_write);
  uint8_t write_result = readRegister(register_to_write);

  if (write_result == data_to_write)
    {
      int succes=0;
    }
  else
    {
      int failure=0;
    }

  // wait a short time to not spam the interface
  delay(10);

}
