# Assignment 2.4b — TIMER0: Polling the IF Overflow Flag

## Overview
Building on 2.4a, you will now let the **hardware** detect the 1-second boundary
instead of comparing `CNT` in software.  Program `TIMER0->TOP` to the desired
tick count; the timer resets automatically when it reaches `TOP` and sets
**IF bit 0** (the overflow flag) to 0x1.  You poll that flag to know when time is up.

## Why This Is Better Than 2.4a
- The timing edge is detected by dedicated hardware — no risk of a soft
  comparison running a cycle too late.
- `TOP` causes the timer to auto-reset, making it easy to reuse for periodic
  events without manually zeroing `CNT` each time.
- You learn about **sticky flags**: `IF` stays set until you explicitly clear it
  via `IFC` — a concept you must understand before using interrupts in 2.4c.

## Code Structure

| Function | Responsibility |
|---|---|
| `app_init()` | One-time hardware setup: clock enables, CTRL register config, stop/clear, calculate TOP, program TOP |
| `app_process_action()` | Start, poll IF, stop, clear IF |

Fill in each `// TODO: your code here` marker.

## Your Tasks

### In `app_init()`
**Step 1 — Enable clocks**
- Enable the HFPER clock bus and then the TIMER0 peripheral clock.

**Step 2 — Configure CTRL**
- Set the prescaler to 1024 into `CTRL[27:24]` with a read-modify-write.
- Set DEBUGRUN (`CTRL[6] = 1`).

**Step 3 — Stop and zero**
- Stop the timer and clear `CNT` before the first run.

**Step 4 — Read the clock frequency**
- Call `CMU_ClockFreqGet(cmuClock_TIMER0)`.

**Step 5 — Calculate target ticks and program TOP**
- Formula: `target_ticks = duration * timerClock / prescaler`
- Write the result to `TIMER0->TOP`.
- Reset `TIMER0->CNT = 0` for a clean measurement.

### In `app_process_action()`
**Step 6 — Start the timer**
- Write the START bit (bit 0) of `TIMER0->CMD`.

**Step 7 — Poll the overflow flag**
- Spin while `(TIMER0->IF & 0x1) == 0`.
  The flag goes high when CNT reaches TOP and the hardware resets the counter.

**Step 8 — Stop the timer**
- Write the STOP bit (bit 1) of `TIMER0->CMD`.

**Step 9 — Clear the IF flag**
- Write 1 to `TIMER0->IFC` bit 0.
- IF flags are **sticky** — if you skip this step, any future interrupt-based
  design will trigger immediately on enable because the flag is already set.

**Step 10 — Inspect and reset** — read `TIMER0->CNT` into a local variable to
inspect overshoot in the debugger, then clear it to 0.

## Key Registers
| Register | Field | Purpose |
|---|---|---|
| `TIMER0->CTRL` | bits [27:24] | Prescaler (÷1024) |
| `TIMER0->CTRL` | bit 6 | DEBUGRUN |
| `TIMER0->TOP` | — | Auto-reset value; overflow fires when CNT reaches this |
| `TIMER0->IF` | bit 0 (OF) | Overflow flag — set by hardware, read-only |
| `TIMER0->IFC` | bit 0 | Write 1 to clear IF bit 0 |
| `TIMER0->CMD` | bit 0 = START, bit 1 = STOP | Start / stop the counter |

## Math Check
48,000,000 / 1024 ≈ **46,875 ticks** for 1 second.

## Reference
- Lecture Notes 2.2: Timers
- EFM32GG Reference Manual, Chapter 20 (TIMER)
- AN14: EFM32 Timer Application Note
