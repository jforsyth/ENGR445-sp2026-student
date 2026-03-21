/***************************************************************************//**
 * @file
 * @brief Assignment 2.4b — Wait for TIMER0 overflow using the IF flag
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

  // Step 4: Read back the actual peripheral clock frequency.
  // This lets us calculate tick counts without hard-coding magic numbers.
  // On this board the HFPER clock is 48 MHz.
  unsigned int timerClock = CMU_ClockFreqGet(cmuClock_TIMER0);

  // Step 5: Calculate the target tick count and program the TOP register.
  // Use CMU_ClockFreqGet(cmuClock_TIMER0) to get the actual speed of the hfper
  // clock going into the timer, then calculate:
  // Instead of comparing CNT to a target in software (as in 2.4a), we let
  // the hardware do it.  When CNT reaches TOP it resets to 0 and latches the
  // overflow flag in IF bit 0.
  //   target_ticks = duration * timer_clock / prescaler
  //               = 1 * 48,000,000 / 1024 ≈ 46,875
  // Note: duration = 1 s here, so it is often omitted from the simplified
  // expression.  It is written out explicitly below so the full formula is
  // visible.  Since duration is the first operand, the entire expression is
  // promoted to float — no integer division truncation occurs.
  // The cast to unsigned int floors the result to the nearest whole tick.
  // TODO: your code here
}

/***************************************************************************//**
 * Assignment 2.4b — Polling the TIMER0 Interrupt Flag (IF)
 *
 * Goal: create a precise 1-second delay using TIMER0's hardware TOP register
 * and overflow flag.
 *
 * Improvement over 2.4a: instead of comparing CNT to a calculated value in
 * software, we program the hardware TOP register so the timer resets itself
 * automatically at exactly the right count.  The hardware then sets the
 * overflow bit in the Interrupt Flags (IF) register, which we poll.
 *
 * The CPU still spins in a loop, but the timing edge is now controlled by
 * dedicated hardware rather than a software comparison.
 ******************************************************************************/
void app_process_action(void)
{
  // Step 6: Start the timer
  // Writing bit 0 of CMD sets the START bit.
  // TODO: your code here

  // Step 7: Poll the overflow flag
  // IF bit 0 is the OF (overflow) flag — it goes high when CNT wraps at TOP.
  // Unlike the 2.4a loop that compared CNT in software, this loop simply
  // reads a hardware-latched status bit — cleaner and immune to soft timing.
  while ((TIMER0->IF & 0x1) == 0) {}

  // Step 8: Stop the timer
  // TODO: your code here

  // Step 9: Clear the IF flag
  // IF flags are sticky — they stay set until explicitly cleared via IFC.
  // Writing a 1 to IFC bit 0 clears the corresponding IF bit.
  // If you forget this, a future interrupt-based design will trigger
  // immediately on enable because the flag is already set.
  // TODO: your code here

  // Step 10: Read and examine the final CNT value
  // It should be should be 0 (or 1) because the timer just reset.
  // This dummy variable serves as a breakpoint for the autograder.
  // TODO: read TIMER0->CNT into a local variable

}
