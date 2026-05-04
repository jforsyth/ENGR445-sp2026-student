/*
 * LIS23DH.h
 *
 *  Created on: Jul 9, 2017
 *      Author: Jason Forsyth
 *
 *     Modified: April 15, 2024 for
 *     ADXL345 in ENGR 445
 */

#ifndef ADXL345_H_
#define ADXL345_H_

/*
 * Necessary includes
 */
#include <stdint.h>
#include <assert.h>

/*
 * Include EMLIBs
 */
#include <em_device.h>
#include <em_chip.h>
#include <em_cmu.h>
#include <em_emu.h>
#include <em_gpio.h>
#include <em_timer.h>
#include <em_usart.h>

/*
   DEFINE REGISTER ADDRESSES
   Useful when calling registers such as readRegister(DATA_FORMAT)
 */
#define DEVICE_ID_REG_ADDRESS 0x0
#define BW_RATE 0x2C
#define POWER_CLT 0x2D
#define DATA_FORMAT 0x31
#define X_DATA_0 0x32
#define X_DATA_1 0x33
#define Y_DATA_0 0x34
#define Y_DATA_1 0x35
#define Z_DATA_0 0x36
#define Z_DATA_1 0x37

class ADXL345 {
public:
  ADXL345();
  virtual ~ADXL345();

  /*
   * Initialize the clocks, GPIO, SPI, and ADXL345 to default values
   *
   *@return result returns true if initialization completed successfully, otherwise false
   */
  bool initialize();

  /*
   * Perform a self-test by reading the DEVICE_ID Register
   * @return returns true is self-test was completed successfully, otherwise false
   */
  bool selfTest();

  /*
   * Get X-axis acceleration from ADXL345 and 16-bit signed value
   * @return x-axis acceleration
   */
  int16_t getXAcceleration();

  /*
   * Get Y-axis acceleration from ADXL345 and 16-bit signed value
   * @return y-axis acceleration
   */
  int16_t getYAcceleration();


  /*
   * Get Z-axis acceleration from ADXL345 and 16-bit signed value
   * @return z-axis acceleration
   */
  int16_t getZAcceleration();

  /**
   * Convert int16_t reading into floating. Assumes +/-2g range and 1mg/digit
   * point acceleration value
   * @param reading 2's complement signed value to be converted in a floating point acceleration
   * @return floating point value expressing an acceleration
   */
  float convertReadingToAccel(int16_t reading);

  //enumeration to specify what acceleration range to measure
  enum ADXL_RANGE {TWO_G, FOUR_G, EIGHT_G, SIXTEEN_G};

  //enumeration to specify whether 10-bit or full 13-bit resolution should be applied.
  enum ADXL_RESOLUTION {NORMAL_RESOLUTION, FULL_RESOLUTION};

private:

  /**
   * Read a single register and return its value
   * @param address 8-bit address of internal register to read
   * @return  the value that was contained in that register
   */
  uint8_t readRegister(uint8_t address);

  /**
   * Write a single register
   * @param address 8-bit address of internal register to read
   * @param data 8-bit data value that should be written to the register
   */
  void writeRegister(uint8_t address , uint8_t data);

  /**
   * Conduct transfer on SPI bus with included data
   * @param data 8-bit data that should be transmitted over the SPI bus
   * @return received 8-bits as result of transmission
   */
  uint8_t SPI_Transfer(uint8_t data);

  /**
   * Helper function to setup the associated clocks
   */
  void setupClocks();

  /**
   * Helper function to setup the associated GPIO
   */
  void setupGPIO();

  /**
   * Helper function to setup the USART
   * for SPI operation
   */
  void setupSPI();

  /**
   * Setup accelerometer to default parameters
   * defined in project specification
   */
  void setupADXL345();

  /**
   * Handle to my USART. Enables to be used with any USART
   * in the system. Will be set to USART1 for now. But will be a
   * place holder for future expansion
   */
  USART_TypeDef* usart = USART1;


};

#endif
