/***************************************************************************//**
 * @file
 * @brief Assignment 2.4a — Measure a 1-second delay by polling TIMER0->CNT
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

#include <em_timer.h>
#include <em_cmu.h>

/***************************************************************************//**
 * app_init — one-time hardware setup
 *
 * Everything that configures the peripheral goes here so that
 * app_process_action() only contains the run-time behaviour.
 ******************************************************************************/
void app_init(void)
{
  // Step 1: Gate the clocks
  // The High-Frequency Peripheral clock bus must be enabled before any
  // peripheral on it (GPIO, TIMER, USART, …) will respond.
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_TIMER0, true);

  // Step 2: Configure TIMER0 CTRL register
  // Defaults are fine for MODE (count up) and CLKSEL (HFPER).
  // We only need to change the prescaler and enable DEBUGRUN.

  // Prescaler — bits [27:24] in CTRL. Set this to divide-by-1024.
  // Why prescale? A 48 MHz counter rolls over (65535 counts) every ~1.4 ms.
  // With a 1024 prescaler each tick is ~21 µs, giving us ~1.4 seconds of
  // headroom before overflow — enough for a 1-second target.
  // TODO: your code here

  // Step 3: Stop and zero
  // Make sure the timer is stopped and CNT is zeroed before the first run.
  // TODO: your code here
}

/***************************************************************************//**
 * Assignment 2.4a — Polling TIMER0->CNT
 *
 * Goal: create an approximately 1-second delay using TIMER0.
 * Strategy: start the timer, spin until TIMER0->CNT reaches the calculated
 * tick count, then stop it. This is the simplest (but most CPU-wasteful)
 * approach — the processor does nothing useful while sitting in the loop.
 ******************************************************************************/
void app_process_action(void)
{
  // Step 4: Read back the actual peripheral clock frequency.
  // This lets us calculate tick counts without hard-coding magic numbers.
  // On this board the HFPER clock is 48 MHz.
  unsigned int timerClock = CMU_ClockFreqGet(cmuClock_TIMER0);

  // Step 5: Calculate the target tick count
  // Use CMU_ClockFreqGet(cmuClock_TIMER0) to get the actual speed of the hfper
  // clock going into the timer, then calculate:
  //   target_ticks = duration * timer_clock / prescaler
  //               = 1 * 48,000,000 / 1024
  //               ≈ 46,875
  // Note: duration = 1 s here, so it is often omitted from the simplified
  // expression (multiplying by 1 is a no-op).  It is written out explicitly
  // below so the full formula is visible.  Since duration is the first operand,
  // the entire expression is promoted to float — no integer division truncation
  // occurs.  The cast to unsigned int floors the result to the nearest whole tick.
  // Verify your calculation: the result should be within 1 tick of your manual calculation.
  // TODO: your code here

  // Step 6: Start the timer
  // Writing bit 0 of CMD sets the START bit.
  // TODO: your code here

  // Step 7: Spin until CNT reaches the target
  // This is the "poll" — the CPU loops here doing nothing else.
  // Note: using <= instead of == avoids missing the exact value if the
  // debugger or an interrupt briefly delays us.
  while (TIMER0->CNT <= targetTicks) {}

  // Step 8: Stop the timer
  // Writing bit 1 of CMD sets the STOP bit.
  // TODO: your code here

  // Step 9: Read and examine the final CNT value
  // It should be targetTicks+1 at most — overshoot is minimal.
  // This dummy variable serves as a breakpoint for the autograder.
  // TODO: read TIMER0->CNT into a local variable

}
