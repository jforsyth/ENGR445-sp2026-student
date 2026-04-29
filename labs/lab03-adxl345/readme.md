# Lab 03 — ADXL345 Accelerometer Class

## Overview
In this milestone you will implement a custom C++ class to interface with your ADXL345 accelerometer via the SPI interface. This lab brings together all the low-level peripheral knowledge you've developed in previous assignments and organizes it into a reusable, object-oriented design.

## Prerequisites
This assignment builds directly on:
- **Assignment 3.2 (SPI Loopback)** — You learned how to:
  - Configure USART1 for synchronous SPI mode 
  - Set up GPIO pins (PD0–PD3) for MOSI, MISO, CLK, and CS
  - Enable peripheral clocks (HFPER, GPIO, USART1)
  - Implement `SPI_Transfer()` to send and receive bytes
  - Validate SPI communication with a hardware loopback test

- **Assignment 3.3a (SPI Read/Write)** — You built on SPI Loopback to add:
  - `readRegister()` — Read a single byte from an ADXL345 register
  - `writeRegister()` — Write a single byte to an ADXL345 register
  - Proper CS (chip select) management for real device communication
  - Validation by reading the ADXL345 DEVICE_ID register (0xE5)

You already have all the working code needed to talk to the ADXL345. Now you'll **organize it into a C++ class** to create a clean, reusable interface.

## Deliverable
Complete the implementation of the **ADXL345** class that:
1. Initializes the SPI peripheral and GPIO pins
2. Configures the ADXL345 accelerometer for measurement
3. Reads and writes ADXL345 registers via SPI
4. Acquires X, Y, and Z acceleration measurements

**Submit:** Your completed `ADXL345.h` and `ADXL345.cpp` files.

**Academic Integrity:** This is considered a closed assignment. Students are not permitted to collaborate with others, seek solutions from resources outside of the class, or utilize generative AI tools in any manner. Solutions suspected to violate these parameters will be required to explained, justified, or re-written during an in-person examination using a closed computer system.

---

## Tasks

Much of the class structure has already been implemented for you. Your job is to complete the following class methods (functions):

### Methods to Implement:
1. **`setupClocks()`** — Enable clocks for GPIO and USART1 (copy from Assignment 3.2/3.3a)
2. **`setupGPIO()`** — Configure PD0–PD3 for SPI operation (copy from Assignment 3.2/3.3a)
3. **`setupSPI()`** — Configure USART1 for SPI mode (copy from Assignment 3.2/3.3a)
4. **`SPI_Transfer(uint8_t data)`** — Perform a single SPI byte transfer (copy from Assignment 3.2/3.3a)
5. **`writeRegister(uint8_t address, uint8_t data)`** — Write a value to an ADXL345 register (copy from Assignment 3.3a)
6. **`readRegister(uint8_t address)`** — Read a value from an ADXL345 register (copy from Assignment 3.3a)
7. **`setupADXL345()`** — Configure the ADXL345 for measurement mode:
   - Write `0x08` to `POWER_CLT` (0x2D) to enable measurement mode
   - Write `0x00` to `DATA_FORMAT` (0x31) to set ±2g range, 10-bit resolution
8. **`getXAcceleration()`** — Read X-axis acceleration (registers `X_DATA_0` and `X_DATA_1`)
9. **`getYAcceleration()`** — Read Y-axis acceleration (registers `Y_DATA_0` and `Y_DATA_1`)
10. **`getZAcceleration()`** — Read Z-axis acceleration (registers `Z_DATA_0` and `Z_DATA_1`)

---

## How the Class is Used in `app.cpp`

The provided `app.cpp` demonstrates how your class will be instantiated and called:

### 1. **Global Instance Creation**
```cpp
#include "ADXL345.h"

// Create a static instance of the ADXL345 class
ADXL345 accel;
```
This line creates a global object named `accel` of type `ADXL345`. When this object is created, the **constructor** `ADXL345()` is automatically called (you don't need to implement anything special here — the default constructor is sufficient).

### 2. **Initialization in `app_init()`**
```cpp
void app_init(void) {
    // Step #1: Initialize the ADXL345
    accel.initialize();

    // Step #2: Perform a self-test
    bool success = accel.selfTest();

    // Infinite loop if self-test fails
    while(!success) {}
}
```
- **`accel.initialize()`** calls your class's public `initialize()` method, which internally calls:
  - `setupClocks()` — Enable peripheral clocks
  - `setupGPIO()` — Configure GPIO pins for SPI
  - `setupSPI()` — Configure USART1 for SPI mode
  - `setupADXL345()` — Configure the ADXL345 accelerometer
  
- **`accel.selfTest()`** verifies communication by reading the `DEVICE_ID` register (0x0), which should return `0xE5`. If it fails, the program halts.

### 3. **Reading Data in `app_process_action()`**
```cpp
void app_process_action(void) {
    int16_t xReading = accel.getXAcceleration();
    int16_t yReading = accel.getYAcceleration();
    int16_t zReading = accel.getZAcceleration();

    int32_t magnitude = sqrt(xReading*xReading + yReading*yReading + zReading*zReading);
    float g_magnitude = magnitude / 256.0;

    int dummy = 0;  // Breakpoint here to inspect g_magnitude
}
```
- Each call to `getXAcceleration()`, `getYAcceleration()`, or `getZAcceleration()` reads two registers from the ADXL345 and combines them into a 16-bit signed value.
- The magnitude is calculated using the Pythagorean theorem: √(x² + y² + z²)
- The result is divided by 256 to convert from raw units to "g" (1g = 256 units at default ±2g range)
- **Expected Result:** When the board is stationary on a table, the magnitude should be approximately **1.0g** (gravity)

---

## C++ Class Primer

If you haven't worked with C++ classes before, here's a quick introduction:

### What is a Class?
A **class** is a blueprint for creating objects that bundle together:
- **Data** (member variables)
- **Functions** (methods) that operate on that data

Think of it like a template: the class `ADXL345` defines what every ADXL345 accelerometer object should know (its USART handle) and what it can do (read/write registers, get acceleration).

### Class vs. Object
- **Class** = The blueprint (defined in `ADXL345.h` and `ADXL345.cpp`)
- **Object** = An actual instance created from the blueprint (e.g., `ADXL345 accel;`)

### Public vs. Private
Classes have two main sections:
- **`public:`** — Functions and variables that can be accessed from outside the class (like `initialize()`, `getXAcceleration()`)
- **`private:`** — Functions and variables that are internal implementation details (like `setupClocks()`, `SPI_Transfer()`, the `usart` handle)

**Why hide things as private?** It prevents other code from accidentally breaking your class's internal state. Only the class's own methods can call private functions or access private variables.

### Header (.h) vs. Implementation (.cpp)
- **`ADXL345.h`** — Declares the class structure (what methods exist, what variables it has)
- **`ADXL345.cpp`** — Implements the actual code for each method

### Method Syntax
When implementing a method in the `.cpp` file, you need to tell C++ which class it belongs to:
```cpp
// In ADXL345.cpp
uint8_t ADXL345::readRegister(uint8_t address) {
    //        ^^^^^ says "this is the readRegister method of the ADXL345 class"
    // Your code here
}
```

### The `this` Pointer (Implicit)
Inside a class method, you can access member variables directly:
```cpp
uint8_t ADXL345::SPI_Transfer(uint8_t data) {
    usart->TXDATA = data;  // Accesses the 'usart' member variable
    // ...
}
```
C++ automatically knows you mean "this object's `usart` variable."

### Constructor and Destructor
- **Constructor** (`ADXL345()`) — Called automatically when an object is created. Used for initialization.
- **Destructor** (`~ADXL345()`) — Called automatically when an object is destroyed. Used for cleanup.

For this lab, the default constructor and destructor are fine — you don't need to add anything special to them.

---

## Testing Your Code

1. **Build and flash** your code to the Giant Gecko board
2. **Set a breakpoint** at the line `int dummy = 0;` in `app_process_action()`
3. **Run the debugger** and check the value of `g_magnitude`:
   - It should be close to **1.0** (representing 1g of gravitational acceleration)
   - If it's 0 or nonsensical, check your `getXAcceleration()`, `getYAcceleration()`, and `getZAcceleration()` implementations
4. **Tilt the board** and step through the debugger again — the x, y, and z values should change as you tilt different axes

---

## Common Pitfalls

1. **Forgetting to call `setupADXL345()`** — The ADXL345 powers on in standby mode. You must write to `POWER_CLT` to enable measurement mode.
2. **Wrong byte order** — Remember the ADXL345 stores data as little-endian (low byte first, then high byte). Use `(high_byte << 8) | low_byte` to combine them correctly.
3. **Not enabling clocks** — If you forget `CMU_ClockEnable()` for GPIO or USART1, the peripherals won't work.
4. **CS pin polarity** — The ADXL345 CS (chip select) is active-low. Make sure you're driving PD3 low during transfers and high when idle.