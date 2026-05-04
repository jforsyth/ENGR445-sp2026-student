/*
 * UARTSerial.cpp
 *
 * Hardware-in-the-Loop UART Data Source Implementation
 * Receives acceleration data from PC via UART
 *
 * Created: May 2026
 * For ENGR 445 Lab 04
 */

#include "UARTSerial.h"
#include <cstring>
#include <cstdlib>

// Global pointer for interrupt handler access
UARTSerial* g_uartSerial = nullptr;

UARTSerial::UARTSerial() :
    x_accel(0),
    y_accel(0),
    z_accel(0),
    new_data_flag(false),
    rx_index(0)
{
  memset(rx_buffer, 0, sizeof(rx_buffer));
  g_uartSerial = this;
}

UARTSerial::~UARTSerial() {
  g_uartSerial = nullptr;
}

bool UARTSerial::initialize() {
  /* UART0 is a HFPERCLK peripheral. Enable HFPERCLK domain and UART0.
   * We also need to enable the clock for GPIO to configure pins. */
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_UART0, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Initialize UART0 for 115200 baud, 8N1
  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
  init.baudrate = 115200;
  USART_InitAsync(UART0, &init);

  // Configure GPIO pins for UART0
  GPIO_PinModeSet(gpioPortE, 0, gpioModePushPull, 1);    // PE0 TX
  GPIO_PinModeSet(gpioPortE, 1, gpioModeInput, 0);       // PE1 RX
  
  // Configure UART0 routing to location 1 (PE0=TX, PE1=RX)
  UART0->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USART_ROUTE_LOCATION_LOC1;

  // Enable VCOM (board-specific requirement for USB-to-UART bridge)
  GPIO_PinModeSet(gpioPortF, 7, gpioModePushPull, 1);

  // Enable RXDATAV interrupt (triggers when receive data is valid)
  UART0->IEN |= (USART_IEN_RXDATAV);

  // Enable UART0 RX interrupt in NVIC
  NVIC_EnableIRQ(UART0_RX_IRQn);

  return true;
}

bool UARTSerial::selfTest() {
  // UART doesn't have an equivalent self-test, so always return true
  return true;
}

int16_t UARTSerial::getXAcceleration() {
  new_data_flag = false; // Clear flag after reading
  return x_accel;
}

int16_t UARTSerial::getYAcceleration() {
  return y_accel;
}

int16_t UARTSerial::getZAcceleration() {
  return z_accel;
}

bool UARTSerial::dataAvailable() {
  return new_data_flag;
}

void UARTSerial::waitForNextLine() {
  // Clear flag and wait for new data
  new_data_flag = false;
  while (!new_data_flag) {
    // Busy wait - interrupt will set the flag when new line arrives
  }
}

void UARTSerial::handleRxInterrupt() {
  // Read received character
  char c = UART0->RXDATA;

  // Check for buffer overflow
  if (rx_index >= (int)sizeof(rx_buffer) - 1) {
    rx_index = 0; // Reset on overflow
  }

  // Add character to buffer
  rx_buffer[rx_index++] = c;

  // Check if complete line received (newline character)
  if (c == '\n' || c == '\r') {
    rx_buffer[rx_index - 1] = '\0'; // Null-terminate string
    parseReceivedLine();
    rx_index = 0; // Reset buffer for next line
  }
}

void UARTSerial::parseReceivedLine() {
  // Expected format: "x,y,z" (e.g., "100,-50,980")
  // Parse the comma-separated values

  if (rx_index == 0) return; // Empty line

  int16_t temp_values[3] = {0, 0, 0};
  int value_index = 0;
  int start = 0;
  
  // Manual parsing to avoid strtok_r
  for (int i = 0; i <= rx_index && value_index < 3; i++) {
    if (rx_buffer[i] == ',' || rx_buffer[i] == '\0' || i == rx_index) {
      rx_buffer[i] = '\0'; // Null-terminate
      temp_values[value_index++] = (int16_t)atoi(&rx_buffer[start]);
      start = i + 1;
    }
  }

  // Only update if we got all 3 values
  if (value_index == 3) {
    x_accel = temp_values[0];
    y_accel = temp_values[1];
    z_accel = temp_values[2];
    new_data_flag = true;
  }
}

/*
 * UART0 RX Interrupt Handler
 * This must be defined at global scope to match the interrupt vector table
 */
extern "C" void UART0_RX_IRQHandler(void) {
  if (g_uartSerial != nullptr) {
    g_uartSerial->handleRxInterrupt();
  }
}
