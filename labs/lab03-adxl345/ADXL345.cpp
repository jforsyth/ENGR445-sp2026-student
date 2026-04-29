/*
 * ADXL345.cpp
 *
 * Created: 10/7/2019
 * Author: Jason Forsyth
 */


#include "ADXL345.h"

// default constructor
ADXL345::ADXL345()
{
  //DO NOTHING HERE
}

// default destructor
ADXL345::~ADXL345()
{
  //DO NOTHING HERE
}

bool ADXL345::initialize()
{
  //configure the required clocks
  setupClocks();

  //configure the required GPIO
  setupGPIO();

  //configure the USART for SPI operation
  setupSPI();

  //configure the ADXL345 to capture sensor data
  setupADXL345();

  return true;
}

/**
 * Setup all the internal clocks of the Giant Gecko
 */
void ADXL345::setupClocks()
{
  //enable the clock for the High-Frequency Peripherals
  CMU_ClockEnable(cmuClock_HFPER, true);

  //enable GPIO clock
  CMU_ClockEnable(cmuClock_GPIO, true);

  //enable the USART1 clock
  CMU_ClockEnable(cmuClock_USART1, true);

}

/**
 * Setup all the GPIO pins for the Giant Gecko
 */
void ADXL345::setupGPIO()
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

/**
 * Perform all operations to configure the USART for SPI operation
 */
void ADXL345::setupSPI()
{

  /**
   * Your code here
   */

}


/**
 * Conduct a single 8-bit transfer on the SPI interface
 * @param data Bits to be sent on SPI interface
 * @return Bits returned from transaction
 */
uint8_t ADXL345::SPI_Transfer(uint8_t data)
{
  /**
   * Your code here
   */

  return 0;

}

/**
 * Perform all initialization of the ADXL itself. At a minimum
 * the MEASURE bit should be set to cause acceleration measurements
 */
void ADXL345::setupADXL345()
{
  /**
   * Your code here
   */
}

/**
 * Write to an internal register of the ADXL345
 * @param address Internal register to be written to
 * @param data Bits that will be written to the register
 */
void ADXL345::writeRegister(uint8_t address, uint8_t data)
{
  /**
   * Your code here
   */

}

/**
 * Read from an internal register of the ADXL 345.
 * @param address Internal ADXL345 register address to read
 * @return Result of read operation
 */
uint8_t ADXL345::readRegister(uint8_t address)
{

  /**
   * Your code here
   */

  return 0x0;

}

bool ADXL345::selfTest()
{

  //implement code to check if you can
  //read the DEVICE_ID register. Return TRUE if
  //the correct result is found, otherwise FALSE.

  uint8_t deviceID = readRegister(DEVICE_ID_REG_ADDRESS);

  if(deviceID == 0xE5)
    {
      return true;
    }
  else
    {
      return false;
    }
}


/*
 * Get X-axis acceleration from ADXL345 and 16-bit signed value
 * @return x-axis acceleration
 */
int16_t ADXL345::getXAcceleration()
{
  /**
   * Your code here
   */

  return 0;
}

/*
 * Get Y-axis acceleration from ADXL345 and 16-bit signed value
 * @return Y-axis acceleration
 */
int16_t ADXL345::getYAcceleration()
{

  /**
   * Your code here
   */

  return 0;
}

/*
 * Get Z-axis acceleration from ADXL345 and 16-bit signed value
 * @return Z-axis acceleration
 */
int16_t ADXL345::getZAcceleration()
{

  /**
   * Your code here
   */

  return 0;
}
