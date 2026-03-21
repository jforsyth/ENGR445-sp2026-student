# Assignment 2.4c — TIMER0: Interrupt-Driven Overflow

## Overview
In this final timer assignment you will stop polling entirely.  Instead, the CPU
is **interrupted automatically** when the timer overflows.  A dedicated handler
function (`TIMER0_IRQHandler`) does the work; the CPU is free to do other things
between timer events.

## How Interrupts Work on Cortex-M3 / EFM32GG

```
TIMER0 overflow
      │
      ▼
TIMER0->IF bit 0 set  (hardware, automatic)
      │
      ▼  only if TIMER0->IEN bit 0 == 1
IRQ line asserted to NVIC
      │
      ▼  only if NVIC->ISER[0] bit 2 == 1
CPU jumps to TIMER0_IRQHandler
```

**Both enables are required.** The peripheral (`IEN`) and the CPU-level (`NVIC`)
enable must both be set — either one alone is not sufficient.

The function name `TIMER0_IRQHandler` is **not arbitrary**.  The startup file
places a pointer to this exact symbol in the vector table.  A typo will silently
fall through to a default infinite-loop handler.

## Code Structure

| Location | Responsibility |
|---|---|
| `TIMER0_IRQHandler()` | Clear IF, stop timer, read CNT |
| `app_init()` | Clock enables, CTRL config, clock read, calculate TOP, stop/clear, enable IEN, enable NVIC, start |
| `app_process_action()` | Empty (all setup in init) |

Fill in each `// TODO: your code here` marker.

## Your Tasks

### In `TIMER0_IRQHandler()`
**Step A — Clear the IF flag first**
- Write 1 to `TIMER0->IFC` bit 0 **before doing anything else**.
- If you leave IF set the CPU will re-enter this handler immediately after
  returning, causing an infinite interrupt storm.

**Step B — Stop the timer**
- Write the STOP bit (bit 1) to `TIMER0->CMD`.

**Step C — Read CNT**
- Read `TIMER0->CNT` into a local variable.
  It should be 0 or 1 because the timer just auto-reset at TOP.

In a real application you would toggle an LED, set a flag, or enqueue a message
here instead of just reading CNT.

### In `app_init()`
**Step 1 — Enable clocks**
- Enable the HFPER clock bus and then the TIMER0 peripheral clock.

**Step 2 — Configure CTRL**
- Set the prescaler to 1024 into `CTRL[27:24]` with a read-modify-write.
- Set DEBUGRUN (`CTRL[6] = 1`).

**Step 3 — Read the clock frequency**
- Call `CMU_ClockFreqGet(cmuClock_TIMER0)`.

**Step 4 — Calculate target ticks and program TOP**
- Formula: `target_ticks = duration * timerClock / prescaler`
- Write to `TIMER0->TOP`, then reset `TIMER0->CNT = 0`.

**Step 5 — Stop and zero**
- Stop the timer and clear `CNT` to ensure clean state.

**Step 6 — Enable the overflow interrupt inside TIMER0**
- Set bit 0 of `TIMER0->IEN`.
  This arms the peripheral to assert an IRQ line to the NVIC when IF bit 0
  goes high — but the CPU won't respond until the NVIC is also enabled.

**Step 7 — Enable the TIMER0 vector in the NVIC**
- Call `NVIC_EnableIRQ(TIMER0_IRQn)`.
  `TIMER0_IRQn` is defined in the device header (included via `em_timer.h`).

**Step 8 — Start the timer**
- Write the START bit (bit 0) of `TIMER0->CMD`.
- The interrupt will fire approximately 1 second later.

### In `app_process_action()`
This function is now **empty**. All setup is complete in `app_init()`. 
The timer runs in the background and will fire `TIMER0_IRQHandler` when the delay expires.
The CPU is free to do other things; the hardware will interrupt it automatically.

## Key Registers / APIs
| Item | Purpose |
|---|---|
| `TIMER0->CTRL` bits [27:24] | Prescaler (÷1024) |
| `TIMER0->CTRL` bit 6 | DEBUGRUN |
| `TIMER0->TOP` | Auto-reset value for overflow |
| `TIMER0->IEN` bit 0 | Arms peripheral to assert IRQ line |
| `NVIC_EnableIRQ(TIMER0_IRQn)` | CMSIS helper to unmask the vector in NVIC |
| `TIMER0->IFC` bit 0 | Clear the sticky OF flag (do this first in the ISR) |
| `TIMER0->CMD` bit 0 / bit 1 | START / STOP |

## Math Check
48,000,000 / 1024 ≈ **46,875 ticks** for 1 second.

## Reference
- Lecture Notes 2.2: Timers
- Lecture Notes 2.3: Interrupts
- EFM32GG Reference Manual, Chapter 20 (TIMER)
- ARMv7-M Architecture Reference Manual, Chapter B3 (NVIC)
- AN14: EFM32 Timer Application Note
