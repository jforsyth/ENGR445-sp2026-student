/*
 * UARTSerial.h
 *
 * Hardware-in-the-Loop UART Data Source
 * Receives acceleration data from PC via UART in format: "x,y,z\n"
 * Provides same interface as ADXL345 for seamless integration
 *
 * Created: May 2026
 * For ENGR 445 Lab 04
 */

#ifndef UARTSERIAL_H_
#define UARTSERIAL_H_

#include <stdint.h>

/*
 * Include EMLIBs
 */
#include <em_device.h>
#include <em_cmu.h>
#include <em_gpio.h>
#include <em_usart.h>

class UARTSerial {
public:
  UARTSerial();
  virtual ~UARTSerial();

  /*
   * Initialize UART0 for receiving acceleration data from PC
   * @return returns true if initialization completed successfully
   */
  bool initialize();

  /*
   * Perform a self-test (always returns true for UART mode)
   * @return returns true (UART has no self-test equivalent)
   */
  bool selfTest();

  /*
   * Get X-axis acceleration from UART data buffer
   * @return x-axis acceleration (last received value)
   */
  int16_t getXAcceleration();

  /*
   * Get Y-axis acceleration from UART data buffer
   * @return y-axis acceleration (last received value)
   */
  int16_t getYAcceleration();

  /*
   * Get Z-axis acceleration from UART data buffer
   * @return z-axis acceleration (last received value)
   */
  int16_t getZAcceleration();

  /*
   * Check if new data has been received since last read
   * @return true if new data available
   */
  bool dataAvailable();

  /*
   * Wait (block) until the next line of data is received from UART
   */
  void waitForNextLine();

  /*
   * Handle UART RX interrupt - should be called from UART0_RX_IRQHandler
   */
  void handleRxInterrupt();

private:
  // Latest acceleration values received from UART
  volatile int16_t x_accel;
  volatile int16_t y_accel;
  volatile int16_t z_accel;

  // Flag indicating new data has been received
  volatile bool new_data_flag;

  // Buffer for incoming UART data
  char rx_buffer[64];
  int rx_index;

  /*
   * Parse received line and extract x, y, z values
   * Expected format: "x,y,z\n" (e.g., "100,-50,980\n")
   */
  void parseReceivedLine();
};

// Global instance for interrupt handler access
extern UARTSerial* g_uartSerial;

#endif /* UARTSERIAL_H_ */
