# Assignment 2.4a — TIMER0: Polling CNT

## Overview
In this assignment you will configure TIMER0 to count at a known rate, start it,
and spin in a loop reading `TIMER0->CNT` until the counter reaches a value that
represents a 1-second elapsed time.  This is the simplest possible way to use a
hardware timer, but it wastes 100% of CPU time during the wait.

## Code Structure
Your `app.cpp` is split into two functions — follow this pattern for all
peripheral-based assignments:

| Function | Responsibility |
|---|---|
| `app_init()` | One-time hardware setup: clock enables, CTRL register config, initial stop/clear |
| `app_process_action()` | Run-time behaviour: calculate target, start, poll, stop |

The skeleton already has the step comments and explanations.  Fill in the code
at each `// TODO: your code here` marker.

## Your Tasks

### In `app_init()`
**Step 1 — Enable clocks**
- Enable the High-Frequency Peripheral (`HFPER`) clock bus via `CMU_ClockEnable`.
- Enable the TIMER0 peripheral clock via `CMU_ClockEnable`.

**Step 2 — Configure CTRL**
- Set the prescaler to 1024 into `TIMER0->CTRL` bits [27:24].
  Use a read-modify-write so you don't clobber other fields.
- Set DEBUGRUN: set bit 6 of `TIMER0->CTRL` so the timer keeps counting
  when the CPU is halted in the debugger.

**Step 3 — Stop and zero**
- Write the STOP command to `TIMER0->CMD` (bit 1) and clear `TIMER0->CNT`
  so the timer is in a known state before the first run.

### In `app_process_action()`
**Step 4 — Read the clock frequency**
- Call `CMU_ClockFreqGet(cmuClock_TIMER0)` and store the result.
  This gives you the actual peripheral frequency (48 MHz on this board)
  without hard-coding a magic number.

**Step 5 — Calculate the target tick count**
- Use the formula: `target_ticks = duration * timerClock / prescaler` from the Timer Lecture Notes.
- A `float` duration variable lets you express fractional seconds later;
  cast the result to `unsigned int` to floor it to whole ticks.
- **Verification**: Manually calculate the ticks (1 × 48,000,000 ÷ 1024)
  and compare to the value your code calculates. They should match exactly
  or differ by at most 1 tick due to rounding.

**Step 6 — Start the timer**
- Write the START bit (bit 0) of `TIMER0->CMD`.

**Step 7 — Poll CNT**
- Spin in a `while` loop until `TIMER0->CNT` reaches `targetTicks`.
- Use `<=` rather than `==` to avoid missing the exact count if an interrupt
  or the debugger briefly delays execution.

**Step 8 — Stop the timer**
- Write the STOP bit (bit 1) of `TIMER0->CMD`.

**Step 9 — Inspect and reset**
- Read `TIMER0->CNT` into a `volatile` variable so you can inspect it in the
  debugger (the overshoot should be at most 1 tick).
- Clear `TIMER0->CNT` to 0 so the next call starts cleanly.

## Key Registers
| Register | Field | Purpose |
|---|---|---|
| `CMU->HFPERCLKEN0` | bit 5 for TIMER0 | Enables peripheral clock |
| `TIMER0->CTRL` | bits [27:24] | Prescaler (÷1024) |
| `TIMER0->CTRL` | bit 6 | DEBUGRUN |
| `TIMER0->CMD` | bit 0 = START, bit 1 = STOP | Start / stop the counter |
| `TIMER0->CNT` | — | Current counter value |

## Math Check
With a 48 MHz clock and a 1024 prescaler, each tick is 1/48,000,000 × 1024 ≈ 21.3 µs.
Target ticks for 1 second ≈ 48,000,000 / 1024 ≈ **46,875**.

## Reference
- Lecture Notes 2.2: Timers
- EFM32GG Reference Manual, Chapter 20 (TIMER)
- AN14: EFM32 Timer Application Note
