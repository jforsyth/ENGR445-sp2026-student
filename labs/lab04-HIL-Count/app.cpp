
//=============================================================================
// MODE CONFIGURATION
// Toggle between desktop development mode and live hardware mode
//=============================================================================
//#define DESKTOP_MODE  // Uncomment for desktop mode (UART input/output)
// Comment out for live mode (ADXL input, LCD output)

//include vectors and math
#include <cmath>
#include <cstdio>
#include "segmentlcd.h"
#include <em_chip.h>

//define appropriate namespace
using namespace std;

// Conditional includes and objects based on mode
#ifdef DESKTOP_MODE
#include "UARTSerial.h"
UARTSerial accel;      // Desktop: UART for input/output
#else
#include "ADXL345.h"
#include "PeriodicTimer.h"
ADXL345 accel;         // Live: ADXL345 sensor
PeriodicTimer timer;   // Live: Timer for periodic sampling
// LCD will be added later
#endif

/**
 * Output results to LCD (stub - will be implemented later)
 */
void reportToLCD(int steps) {
  // Display step count on LCD: "Steps: <VALUE>"
  char buffer[10];
  snprintf(buffer, sizeof(buffer), "%d", steps);
  SegmentLCD_Write("STEPS");
  SegmentLCD_Number(steps);
}

/**
 * Output results to PC via UART (stub - format will be provided later)
 */
void reportToPC(int steps) {
  // Convert steps to string and send via UART0
  // Format: "Steps: X\n" (human readable)

  char buffer[64];
  int len = snprintf(buffer, sizeof(buffer), "Steps: %d\n", steps);

  // Send each character via UART0
  for (int i = 0; i < len; i++) {
      // Wait for TX buffer to be ready
      while (!(UART0->STATUS & UART_STATUS_TXBL));
      UART0->TXDATA = (uint8_t)buffer[i];
  }
}

/**
 * Send ACK character to PC for handshaking
 * Signals that one line has been processed and ready for next line
 */
void sendAck(void) {
  // Send single '+' character as ACK
  while (!(UART0->STATUS & UART_STATUS_TXBL));
  UART0->TXDATA = (uint8_t)'+';
}

/**
 * Initialize all global/static variables
 */

const int ARRAY_LENGTH = 16;
static int mags[ARRAY_LENGTH];
static int diffs[ARRAY_LENGTH];
static int squares[ARRAY_LENGTH];
static int sums[ARRAY_LENGTH];

//circular buffer index
int idx = 0;

//current step count
int mySteps = 0;

//any FSM data that might be needed
enum STATE {
  IDLE, DETECT, TIMEOUT
};

STATE currentState = IDLE;  // Start in IDLE state waiting for step
STATE nextState = IDLE;

/**
 * Place all operations here that will be run "once". These include initializing the
 * SPI (for ADXL), UART (for desktop mode), LCD (for live mode), and TIMER interfaces
 */
void app_init(void)
{

  //enable the clock for the High-Frequency Peripherals
  CMU_ClockEnable(cmuClock_HFPER, true);

  //enable the specific clock for the TIMER0
  CMU_ClockEnable(cmuClock_TIMER0, true);

  CHIP_Init();
  SegmentLCD_Init(false);
  SegmentLCD_Number(1234);

#ifdef DESKTOP_MODE
  //Desktop mode: initialize UART for input/output
  accel.initialize();

  bool success = accel.selfTest();

  SegmentLCD_Write("HIL");
#else
  //Live mode: initialize ADXL345, timer, and LCD
  //Initialize LCD first so we can display errors
  CMU_ClockEnable(cmuClock_LCD, true);
  SegmentLCD_Init(false);

  accel.initialize();

  bool success = accel.selfTest();

  if (!success) {
    //Display ADXL failure on LCD
    SegmentLCD_Write("ADXL");
    SegmentLCD_Number(-1); // or use a different error code
    while(1) {} // Halt
  }

  //initialize periodic timer
  timer.initialize();

  //Display startup message
  SegmentLCD_Write("STEPS");
#endif

  /* Turn on board LEDS for debugging purpose */
  GPIO_PinModeSet(gpioPortE,                /* GPIO port */
                  2,                        /* GPIO port number */
                  gpioModePushPull,         /* Pin mode is set to push pull */
                  0);                       /* High idle state */


                  /**
     * Step 0: Initial all arrays defined above to known values. Really should be 0.
     * Will use FOR LOOP that is very inefficient but is guaranteed to work on all systems
     */

    for(int i=0;i<ARRAY_LENGTH;i++)
    {
      mags[i] = 0;
      diffs[i] = 0;
      squares[i] = 0;
      sums[i] = 0;
    }

     // Initialize the states of the machine to the initial one
    currentState = IDLE;
    nextState = IDLE;
}


/**
 * Place all code that will happen "everytime" in this block. Should include
 * - Acquiring Data from ADXL345 (or UART in desktop mode)
 * - Processing Data in Datapath
 * - Determining whether a step has occurred via FSM
 * - Reporting results (LCD in live mode, UART in desktop mode)
 */

void app_process_action(void)
{

  

  /**
   * Step 1: Acquire data from input source (ADXL345 or UART)
   */
  int32_t xAccel = accel.getXAcceleration();
  int32_t yAccel = accel.getYAcceleration();
  int32_t zAccel = accel.getZAcceleration();

  /**
   * Step 2: Put new acceleration data through data path
   */

  //YOUR DATA PATH CODE HERE.

  /**
   * Step 3: Decide whether a step has occurred via FSM
   */

 // YOUR FSM CODE HERE.



  /**
   * Step 4: Report results based on mode
   */
#ifdef DESKTOP_MODE
  reportToPC(mySteps);
  sendAck();  // Signal to PC that line was processed
  SegmentLCD_Number(idx);
#else
  reportToLCD(mySteps);
#endif

  /**
   * Step 5: Wait for next sample
   */
#ifdef DESKTOP_MODE
  // Desktop mode: wait for next line from UART
  accel.waitForNextLine();
#else
  // Live mode: wait using timer (1s = 1000 ms)
  timer.wait(1000);
#endif

  // toggle the on-board LED for debugging purposes
  GPIO_PinOutToggle(gpioPortE,2);

  idx++;
}
