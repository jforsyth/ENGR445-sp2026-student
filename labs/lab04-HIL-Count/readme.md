# Lab 04: Step Counter with Hardware-in-the-Loop Testing

## Overview

Implement a **step-counting algorithm** that runs on the EFM32GG microcontroller using a 3-axis accelerometer. The lab uses a **dual-mode architecture** for development and testing:

- **DESKTOP_MODE** (enabled) - Test algorithm offline with pre-recorded acceleration data via HIL GUI
- **LIVE_MODE** (commented out) - Final hardware validation with real ADXL345 sensor + LCD display

## Understanding the Architecture

**This code is intentionally complex to support dual modes.** This section explains why, what it means for you, and what you actually need to implement.

### What is Hardware-in-the-Loop (HIL) Testing?

HIL testing lets you run your embedded code on **real hardware** but substitute **simulated/recorded sensor data** instead of real sensors. This enables:

- **Test algorithms offline** without needing actual hardware sensors
- **Reproduce exact scenarios** using recorded data (deterministic, no randomness)
- **Verify correctness** by comparing computed output to known ground truth
- **Accelerate development** without waiting for real sensor acquisition

### Dual-Mode Architecture

Lab 04 uses compile-time conditional compilation to toggle between two completely different modes:

#### DESKTOP_MODE (HIL Testing) - Uncommented for Testing
```cpp
#define DESKTOP_MODE  // Uncomment for desktop testing
```

| Feature | Value |
|---------|-------|
| **Input** | Acceleration data from Python GUI via UART (`"x,y,z\n"` format) |
| **Output** | Step counts to Python GUI via UART (`"Steps: N\n"` format) |
| **Sensor Class** | `UARTSerial` (reads from USB-UART buffer) |
| **Display** | Console/GUI log (LCD shows lines received) |
| **Use Case** | Development, testing, debugging |
| **Sampling** | Python sends one sample at a time, waits for ACK |

#### LIVE_MODE (Hardware Testing) - Commented Out for Submission
```cpp
// #define DESKTOP_MODE  // Commented out for final submission
```

| Feature | Value |
|---------|-------|
| **Input** | Real ADXL345 accelerometer via SPI |
| **Output** | Step counts to physical LCD display |
| **Sensor Class** | `ADXL345` (reads from real hardware) |
| **Display** | Segment LCD |
| **Use Case** | Final validation on physical hardware |
| **Sampling** | Periodic timer (user defined intervals) |

### Why the Code is Complex

Both `ADXL345` and `UARTSerial` implement the **same interface**, so the algorithm runs identically in both modes:

```cpp
class ADXL345 {                  class UARTSerial {
    int16_t getXAcceleration();      int16_t getXAcceleration();
    int16_t getYAcceleration();      int16_t getYAcceleration();
    int16_t getZAcceleration();      int16_t getZAcceleration();
    bool initialize();               bool initialize();
    bool selfTest();                 bool selfTest();
};                               };
```

The `app.cpp` uses **compile-time conditional compilation** to select which class:

```cpp
#ifdef DESKTOP_MODE
    #include "UARTSerial.h"
    UARTSerial accel;      // <-- Desktop mode: UART
#else
    #include "ADXL345.h"
    ADXL345 accel;         // <-- Live mode: SPI
#endif
```

Then **the same algorithm code** works with either sensor:

```cpp
void app_process_action() {
    int x = accel.getXAcceleration();  // Works in BOTH modes!
    int y = accel.getYAcceleration();
    int z = accel.getZAcceleration();
    
    // ... your algorithm ...
    
    #ifdef DESKTOP_MODE
        reportToPC(steps);   // Send to Python GUI
    #else
        reportToLCD(steps);  // Send to LCD
    #endif
}
```

### Critical Implication: You Must Rebuild Twice

⚠️ **This is the most important thing to understand:**

1. **First build:** Leave `#define DESKTOP_MODE` **uncommented** → Flash to Gecko for testing
2. **Second build:** **Comment out** `#define DESKTOP_MODE` → Flash to Gecko for grading

If you forget to comment out `#define DESKTOP_MODE` before final submission:
- ❌ Your code will try to read from UART instead of ADXL345
- ❌ It will wait for data from Python GUI that never comes
- ❌ It will fail **ALL** grading tests

**Double-check before submitting:** The line must be commented out:
```cpp
// #define DESKTOP_MODE  // ← MUST be commented out for submission!
```

### What You Actually Need to Do

Despite the complexity, **you only need to implement two functions**:

1. **`app_init()`** - Runs once at startup
   - Initialize the underlying system and sensors
   - You just need to set up your FSM state and algorithm parameters
   - Copy in code from previously developed labs and assignments
   
2. **`app_process_action()`** - Main algorithm loop
   - Reads from accelerometer (works in both modes)
   - Computes features (magnitude, differences, etc.)
   - Runs step-detection FSM
   - Reports results (mode-aware via `#ifdef`)
   - Copy in your work from the Desktop step counting implementation

Everything else is **pre-implemented scaffolding** to make both modes work.

**Key Takeaway:** You only need to implement `app_init()` and `app_process_action()`. Everything else is scaffolding.

---

## Files You Need to Modify

| File | Your Role | Status |
|------|-----------|--------|
| **app.cpp** | ✏️ Implement `app_init()` and `app_process_action()` | **TODO** |
| app.h | Read only - function declarations | ✅ |
| **ADXL345.h / ADXL345.cpp** | Copy from previous assignment (assignment32/33) | **TODO** |

## Pre-Implemented Files (Do Not Modify)

| File | Purpose |
|------|---------|
| UARTSerial.h / UARTSerial.cpp | UART interface for desktop mode (receives data from Python GUI) |
| PeriodicTimer.h / PeriodicTimer.cpp | Timer for periodic sampling in live mode |
| segmentlcd.h / segmentlcd.c | LCD display driver |
| em_lcd.h / em_lcd.c | EFM32 LCD peripheral controller |
| main.cpp | Entry point (don't modify) |
| em_usart.c | Source code for USART interface

---

## Testing with HIL GUI

The HIL system uses a Python GUI to send pre-recorded acceleration data to your Gecko for testing. This enables fast iteration without real hardware.

### Quick Start: Run the HIL GUI

From the `HIL/` directory:

```bash
pip install pyserial
python3 send_hil_data.py
```

The GUI will:
1. ✅ Auto-detect your Gecko serial port
2. ✅ Let you select a CSV file with acceleration data (e.g., `HIL/48_steps.csv`)
3. ✅ Send samples one-by-one to your Gecko
4. ✅ Display step counts in real-time

### Handshaking Protocol

The HIL system uses an **ACK (acknowledge) protocol** for reliable per-line communication:

1. **Python sends:** `"x,y,z\n"` (e.g., `"12,-8,113\n"`)
2. **Gecko processes** the sample through your step-detection FSM
3. **Gecko sends:** `"Steps: N\n+"` (step count + ACK signal)
4. **Python receives** and displays: `← Steps: 5  [ACK]`
5. **Python repeats** with the next sample

This keeps both sides synchronized — Python won't send the next sample until your Gecko has acknowledged the previous one.

### Expected Output

With sample data `HIL/48_steps.csv`, you should see output like:

```
[16:45:23] [INFO] Connecting to /dev/ttyUSB0 at 115200 baud...
[16:45:23] [OK] Connected to /dev/ttyUSB0
[16:45:23] [INFO] Reading data from: 48_steps.csv
[16:45:23] [INFO] Sending 48 acceleration samples at 10 Hz

[16:45:24] [INFO] → 1,7,112
[16:45:24] [OK] ← Steps: 0  [ACK]
[16:45:24] [INFO] → 1,7,111
[16:45:24] [OK] ← Steps: 0  [ACK]
...
[16:45:28] [INFO] → -6,7,127
[16:45:28] [OK] ← Steps: 48  [ACK]

[16:45:28] [INFO] ==================================================
[16:45:28] [OK] Transmission complete!
[16:45:28] [OK] All 48 samples sent successfully
[16:45:28] [INFO] ==================================================
```

**Note:** Exact step count depends on your algorithm's threshold and FSM parameters.

---

## Step 1: Copy ADXL345 Code

From your previous assignment (assignment32 or 33), copy:
- `ADXL345.h`
- `ADXL345.cpp`

Into this project directory. The header file is already included in the conditional compilation:

```cpp
#else
    #include "ADXL345.h"
    ADXL345 accel;  // <-- Your ADXL345 implementation
#endif
```

---

## Step 2: Review app_init()

This function runs once at startup and is **mostly pre-implemented**. It handles:
- Clock initialization (CMU)
- LCD and sensor setup (mode-aware)
- Circular buffer array zeroing
- FSM state initialization

**What's already done:**
- ✅ Clock enables (HFPER, TIMER0, LCD)
- ✅ Chip initialization and LCD setup
- ✅ Sensor initialization (ADXL345 or UARTSerial)
- ✅ GPIO setup (board LED)
- ✅ Array zeroing loop
- ✅ FSM state reset to IDLE

---

## Step 3: Implement app_process_action()

This is the **main loop** that runs periodically. The skeleton is provided; **you need to fill in the data path and FSM logic**.

**What's pre-implemented:**
- ✅ Step 1: Read accelerometer data (mode-aware)
- ✅ Step 4: Report results (to GUI or LCD)
- ✅ Step 5: Wait for next sample (mode-aware)

**What you need to implement:**
- ❌ Step 2: **Data path** (marked `//YOUR DATA PATH CODE HERE`)
- ❌ Step 3: **FSM logic** (marked `// YOUR FSM CODE HERE`)

**Step 2: Data Path** — Compute features from raw acceleration data:

**Step 3: FSM Logic** — Detect steps using state machine:


## Step 4: Enable DESKTOP_MODE for Testing

In app.cpp, make sure this line is **uncommented**:

```cpp
#define DESKTOP_MODE  // Uncomment for desktop mode (UART input/output)
```

Then rebuild and flash to the Gecko.

---

## Step 5: Test with HIL GUI

Use the Python GUI to test your implementation with pre-recorded acceleration data (see "Testing with HIL GUI" section above for detailed instructions).

The workflow is:
1. ✅ Your code running in DESKTOP_MODE on the Gecko
2. ✅ HIL GUI auto-detects the Gecko port
3. ✅ GUI sends acceleration samples from CSV file
4. ✅ Your `app_process_action()` processes each sample
5. ✅ Your code sends back step count + ACK
6. ✅ GUI displays results in real-time

**Verify results:** With `HIL/48_steps.csv`, you should detect approximately 48 steps.

---

## Step 6: Disable DESKTOP_MODE for Live Testing

Before final submission, **comment out** the `#define DESKTOP_MODE` line:

```cpp
// #define DESKTOP_MODE  // Disabled for live hardware testing
```

Rebuild and flash to the Gecko. Now it will:
- Read from real ADXL345 sensor (SPI)
- Display step count on LCD
- Use periodic timer for sampling

---


## Key Data Structures

**Circular buffers** (fixed-size arrays with index wrapping):
```cpp
static int mags[ARRAY_LENGTH];    // Computed magnitudes
static int diffs[ARRAY_LENGTH];   // Differences (rolling)
static int squares[ARRAY_LENGTH]; // Squared differences
static int sums[ARRAY_LENGTH];    // Moving sums (threshold detection)
int idx = 0;            // Current circular buffer index
```

**FSM States:**
```cpp
enum STATE {
    IDLE,      // Waiting state - check if sum exceeds threshold
    DETECT,    // Step detected - increment counter, move to cooldown
    TIMEOUT    // Timeout/cooldown state - wait before returning to IDLE
};
STATE currentState = IDLE;
```

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| **HIL GUI won't connect** | Make sure `#define DESKTOP_MODE` is uncommented and code is flashed |
| **No steps detected** | Adjust `threshold` value or review FSM logic |
| **Incorrect step count** | Check circular buffer indexing and magnitude computation |
| **LCD shows nothing in live mode** | Verify DESKTOP_MODE is commented out before flashing |
| **ACK timeout in GUI** | Gecko may be stuck in infinite loop; check FSM state transitions |

---

## Expected Performance

With sample data `HIL/48_steps.csv`, you should detect approximately **48 steps**. Exact count depends on your algorithm's threshold and FSM parameters.

---

## Summary

1. ✅ Copy ADXL345.h/cpp from previous assignment
2. ✅ Implement `app_init()` (minimal changes needed)
3. ✅ Implement `app_process_action()` (your main algorithm)
4. ✅ **Leave `#define DESKTOP_MODE` uncommented** → Test with HIL GUI
5. ✅ **Comment out `#define DESKTOP_MODE`** → Test live hardware and submit

**Remember:** The dual-mode architecture is complex, but your implementation is simple. Focus on `app_process_action()`!