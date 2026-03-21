/***************************************************************************//**
 * @file
 * @brief Assignment 2.4c — 1-second delay using the TIMER0 interrupt handler
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
 * TIMER0 Interrupt Service Routine
 *
 * This function is called automatically by the Cortex-M3 when TIMER0's
 * overflow flag fires AND the interrupt is unmasked in both:
 *   (a) TIMER0->IEN  — the peripheral's own interrupt-enable register
 *   (b) NVIC         — the CPU-level Nested Vectored Interrupt Controller
 *
 * Both must be enabled; either one alone is not sufficient.
 *
 * The name TIMER0_IRQHandler is NOT arbitrary.  The startup file
 * (startup_efm32gg.S) places a pointer to this exact symbol in the vector
 * table at the TIMER0 IRQ slot.  If you misspell it the linker will silently
 * use the default weak handler (an infinite loop) instead.
 ******************************************************************************/
void TIMER0_IRQHandler(void)
{
  // Step A: Clear the peripheral IF flag via IFC BEFORE doing any work.
  // If we leave it set the CPU will re-enter this handler immediately after
  // returning, creating an infinite interrupt storm.
  // Writing 1 to IFC bit 0 clears the OF flag in IF.
  // TODO: your code here

  // Step B: Stop the timer so it doesn't keep firing.
  // TODO: your code here

  // Step C: Read the count — should be 0 (or 1) because the timer just reset.
  // TODO: your code here

  // In a real application you would do your work here:
  //   toggle an LED, enqueue a message, set a flag for the main loop, etc.
}

/***************************************************************************//**
 * app_init — one-time hardware setup
 *
 * All configuration, clock gating, TOP calculation, and interrupt setup happens
 * here.  This is the interrupt-driven pattern: everything is initialized upfront,
 * and the timer starts running automatically. The interrupt handler fires when
 * the delay expires.
 ******************************************************************************/
void app_init(void)
{
  // Step 1: Gate the clocks to HFPER and TIMER0
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_TIMER0, true);

  // Step 2: Configure TIMER0 CTRL
  // Prescaler = 1024 (into bits [27:24] of CTRL)
  // DEBUGRUN = 1 (bit 6) to keep counting while CPU is halted in debugger
  // TODO: your code here

  // Step 3: Read the clock frequency
  // This lets us calculate tick counts without hard-coding magic numbers.
  // On this board the HFPER clock is 48 MHz.
  unsigned int timerClock = CMU_ClockFreqGet(cmuClock_TIMER0);

  // Step 4: Calculate target ticks for 1-second delay and program TOP
  // Use CMU_ClockFreqGet(cmuClock_TIMER0) to get the actual speed of the hfper
  // clock going into the timer, then calculate:
  // target_ticks = duration * timer_clock / prescaler
  //              = 1 * 48,000,000 / 1024 ≈ 46,875
  // TODO: your code here (calculate targetTicks, program TOP, set CNT = 0)

  // Step 5: Stop the timer and reset the count to ensure clean state
  // TODO: your code here

  // Step 6: Enable the TIMER0 overflow interrupt inside the peripheral
  // IEN bit 0 = OF (overflow) interrupt enable.
  // This arms the peripheral to assert an IRQ line to the NVIC when IF bit 0
  // goes high — but the CPU won't respond until the NVIC is also enabled.
  // TODO: your code here

  // Step 7: Enable the TIMER0 interrupt vector in the NVIC
  // NVIC_EnableIRQ is a CMSIS helper that sets the appropriate bit in
  // NVIC->ISER[n].  TIMER0_IRQn == 2 on the EFM32GG, so this writes bit 2
  // of ISER[0] (i.e., 0x4).  The equivalent manual write is:
  //   NVIC->ISER[0] |= (1UL << TIMER0_IRQn);  // same effect
  // TODO: your code here

  // Step 8: Start the timer — the interrupt will fire in ~1 second
  // TODO: your code here
}

/***************************************************************************//**
 * Assignment 2.4c — TIMER0 Interrupt-Driven 1-Second Delay
 *
 * Goal: move the "work" out of a polling loop and into an interrupt handler.
 * The CPU is now free to do other things while waiting for the timer; the
 * hardware will interrupt it exactly when the delay expires.
 *
 * All configuration, clock gating, TOP calculation, and interrupt setup happens
 * in app_init().  This function is now empty — the timer is already running and
 * will fire an interrupt automatically without any polling here.
 ******************************************************************************/
void app_process_action(void)
{
  // All work is now in app_init(). The timer runs in the background and will
  // fire TIMER0_IRQHandler when the delay expires. This function does nothing.
}
