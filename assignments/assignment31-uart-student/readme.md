# Assignment 3.1 — UART Polling (USART1 Loopback)

## Overview
In this assignment you will configure USART1 for serial communication and implement a loopback test by polling the status register. You will transmit single and multiple bytes, then verify they are received correctly via the USART1 RX FIFO.

## Hardware Setup
**Important:** This assignment requires a **hardware loopback connection** to test the UART communication. You must connect the TX and RX pins together using a female-to-female wire:
- Connect **PD0 (USART1 TX)** to **PD1 (USART1 RX)** with a jumper wire.

This creates an internal loopback where transmitted data is immediately received by the same USART, allowing you to verify the transmit and receive paths work correctly. Without this connection, the receive FIFO will remain empty and your code will hang waiting for data.

### EXP Header Pinout Reference
The STK3600/3700/3800 boards use a common EXP (expansion) header. The USART1 pins are located on the left side:

![STK3600/3700/3800 EXP Header Pinout](headers.jpg)
**ASCII Pinout Diagram (Left Edge):**
```
STK EXP Header (Left Side):
┌─────────────────┐
│ 3V3             │  (Power)
│ 5V              │
│ GND             │
│ PD6             │
│ PD5             │
│ PD4             │
│ PD3             │  
│ PD2             │
│ PD1  ← RX       │  << Connect to PD0 with a wire
│ PD0  ← TX       │  << Connect to PD1 with a wire
│ VMCU            │
└─────────────────┘
```
**Key pins for this assignment:**
- **PD0** = USART1 TX (transmit)
- **PD1** = USART1 RX (receive)

**To create the loopback:**
1. Locate the EXP header on your board (right angle connectors on boards.)
2. Find **PD0** and **PD1** on the left edge (bottom pins)
3. Connect them together with a female-to-female jumper wire
4. This allows transmitted data on PD0 to loop back immediately on the RX pin (PD1)

## Code Structure
Your `app.cpp` is split into two functions — follow this pattern for all peripheral-based assignments:

| Function | Responsibility |
|---|---|
| `app_init()` | One-time hardware setup: clock enables, USART configuration, GPIO setup |
| `app_process_action()` | Run-time behaviour: transmit bytes, poll RX status, verify received data |

The skeleton already has the step comments and explanations. Fill in the code at each `// TODO: your code here` marker.

## Your Tasks

### In `app_init()`

**Clock Configuration**
- Enable the High-Frequency Peripheral (`HFPER`) clock bus via `CMU_ClockEnable`.
- Enable the GPIO peripheral clock via `CMU_ClockEnable`.
- Enable the USART1 peripheral clock via `CMU_ClockEnable`.

**USART1 Configuration (9600 baud, 8 data bits, 1 stop bit, no parity)**
- Verify the USART1 clock frequency using `CMU_ClockFreqGet(cmuClock_USART1)` — should be 14 MHz.
- Calculate the clock division (CLKDIV) value using the formula provided (already calculated as 0x168).
- Set the FRAME register for 8 data bits (value 0x5) and 1 stop bit.
- Configure the ROUTE register to use Location #1 (PD0 for TX, PD1 for RX).
- Enable TX and RX pins via the ROUTE register.
- Enable transmit and receive in the CMD register.

**GPIO Configuration**
- Configure PD0 (TX) as a push-pull output.
- Configure PD1 (RX) as a push-pull input`.

### In `app_process_action()`

**Task 1: Single-byte loopback**
- Create a `uint8_t` variable with an arbitrary value (e.g., 0x37).
- Write it to `USART1->TXDATA` to transmit.
- Poll the RXDATAV bit (bit 7) in `USART1->STATUS` until data is available.
- Read the received byte from `USART1->RXDATA`.
- Verify it matches the transmitted value.

**Task 2: Multi-byte loopback**
- Create a `uint8_t` array with two arbitrary values (e.g., {0x25, 0x37}).
- Write the first value to `USART1->TXDATA`.
- Write the second value to `USART1->TXDOUBLE` (left-shifted by 8 bits).
- Poll the RXFULL bit (bit 8) in `USART1->STATUS` until the RX FIFO is full.
- Read both received bytes from `USART1->RXDATA` and `USART1->RXDOUBLE`.
- Verify both match the transmitted values.

**Success Condition**
- Once both tasks complete successfully, enter an infinite loop with a counter increment to prevent code optimization.

## Reference
Consult the Helper.h file for the `readBit()` and `setPinMode()` function signatures. The GPIO.h file contains pin mode definitions.