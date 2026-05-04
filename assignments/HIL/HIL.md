# Hardware-in-the-Loop (HIL) Testing

This directory contains tools for testing the step-counting algorithm using pre-recorded acceleration data without requiring real hardware.

## Quick Start

Install dependencies and run:

```bash
pip install pyserial
python3 send_hil_data.py
```

The GUI will open, auto-detect your Gecko port, and be ready to send acceleration data.

---

## Files

- `send_hil_data.py` - User-friendly GUI for sending acceleration data to the Gecko microcontroller
- `48_steps.csv` - Sample dataset with 48 recorded steps in x,y,z acceleration format

## Setup

### Requirements
- Python 3.6+
- `pyserial` module: `pip install pyserial`
- Gecko device with `DESKTOP_MODE` enabled in app.cpp
- USB connection to Gecko via USB-to-UART (PE0/PE1)

### Install Dependencies

```bash
pip install pyserial
```

**Optional: Use a virtual environment**
```bash
python3 -m venv venv
source venv/bin/activate  # macOS/Linux
venv\Scripts\activate.bat  # Windows
pip install pyserial
```

### Enable Desktop Mode

In `simplicity-workspace/lab04-full-algorithm/app.cpp`, uncomment:
```cpp
#define DESKTOP_MODE  // Uncomment for desktop mode (UART input/output)
```

Rebuild and flash to the Gecko.

### Run the GUI

```bash
python3 send_hil_data.py
```

---

## GUI Overview

The GUI opens with the following sections:

#### Configuration Panel
- **Serial Port** - Automatically detects and selects the Gecko port. Looks for CH340, EFM32, Gecko, or Silicon Labs identifiers. If multiple ports exist, shows a dropdown to manually select. Click "Refresh" to re-scan ports.
- **CSV File** - Browse and select an acceleration data file. Shows the filename once selected.
- **Baud Rate** - Defaults to 115200 (standard for Gecko). Can be changed if needed.

#### Status Panel
- **Connection** - Shows green dot (● Connected) or red dot (● Disconnected)
- **Progress** - Real-time progress bar and sample counter

#### Control Buttons
- **Start Transmission** - Begins sending data to Gecko
- **Stop** - Halts transmission mid-stream (disabled until transmission starts)

#### Output Log
- Real-time log of all communication with timestamps
- Color-coded messages:
  - **Black** - Standard info messages
  - **Green** - Success/OK messages
  - **Orange** - Warnings
  - **Red** - Errors
- Clear Log button to reset the log

## Port Auto-Detection

The GUI automatically detects and selects the Gecko port on startup by looking for:
- **CH340 / CH341** - Common USB-UART bridge chips
- **EFM32 / Gecko** - Silicon Labs Gecko microcontroller identifiers
- **STM32** - Other Cortex-M devices
- **USB Serial** - Generic USB serial devices
- **ttyUSB** - Linux USB device paths

**Excluded devices** (NOT auto-selected):
- **Bluetooth** - Bluetooth ports are explicitly excluded
- **IrDA** - Infrared devices
- macOS **cu.Bluetooth** - Bluetooth serial ports

If the Gecko is the only USB serial device connected, it will be auto-selected. If multiple USB devices are present, the first matching device is selected. You can always manually change it using the dropdown or click "Refresh" to re-scan.

## Handshake Protocol

The HIL sender uses a two-stage handshaking protocol to ensure safety:

### Initial Handshake (Startup)
When you click "Start Transmission":
1. **Connect** - GUI opens serial port to Gecko
2. **Wait for READY** - GUI waits for Gecko to send `"READY\n"` signal
3. **Verify Mode** - If READY received → Gecko is in **DESKTOP_MODE** ✓
4. **Begin** - GUI starts sending acceleration data

If the Gecko does **not** send READY within 3 seconds:
- ❌ Error dialog appears
- ❌ Transmission is aborted
- ❌ Log shows: "Gecko is in LIVE_MODE (not DESKTOP_MODE)" or "not responding"

**Fix:** Make sure to:
1. Uncomment `#define DESKTOP_MODE` in app.cpp
2. Rebuild and flash the code to the Gecko
3. Try again

### Per-Line Handshake (During Transmission)
After the initial READY handshake succeeds:

1. **Send** - Sends one line of acceleration data (x,y,z values) with newline
2. **Process** - Gecko receives, processes, and reports step count
3. **ACK** - Gecko sends single `'+'` character to signal readiness
4. **Repeat** - GUI waits for ACK before sending next line

### Example Communication

**Initial handshake:**
```
[PC connects]
Gecko tx:     "READY\n"
GUI receives: "✓ Gecko is ready (DESKTOP_MODE active)"
```

**Per-line transmission:**
```
GUI sends:    "12,-8,113\n"
Gecko rx:     Receives and processes
Gecko tx:     "Steps: 5\n+"
GUI receives: Displays "← Steps: 5  [ACK]"
              Sends next line
```

## How It Works

The GUI implements a reliable communication protocol:

1. **Initial Check** - Waits for READY to confirm DESKTOP_MODE is active
2. **Send Data** - Sends acceleration samples line-by-line
3. **Process** - Gecko processes each sample through step-detection FSM
4. **Report** - Gecko sends step count and ACK signal
5. **Repeat** - Loop continues until all samples sent

## Data Format

CSV file format (header optional, auto-detected):
```
x,y,z
12,-8,113
15,7,110
-6,2,115
...
```

- Values are 16-bit signed integers (int16_t)
- Comma-separated, whitespace-tolerant
- One sample per line
- 10 Hz sampling rate (100 ms per sample)

## Troubleshooting

| Issue | Solution |
|-------|----------|
| **"No READY signal" on startup** | Gecko is NOT in DESKTOP_MODE. Make sure: 1) `#define DESKTOP_MODE` is uncommented in app.cpp 2) Code is rebuilt and flashed 3) USB cable is connected |
| Handshake timeout (3 sec) | Same as above - Gecko needs DESKTOP_MODE enabled and flashed |
| Timeout on ACK during transmission | Gecko crashed or entered infinite loop. Try: 1) Reset the board 2) Verify FSM logic in app.cpp 3) Check threshold value isn't too high/low |
| Port not auto-selected | The GUI looks for CH340, EFM32, Gecko, or Silicon Labs in port names. If your device isn't recognized, manually select it from the dropdown or check the port description in Device Manager |
| No serial ports available | Check USB cable connection to Gecko, or install CH340 drivers if needed |
| "Permission denied" on Linux/macOS | Run `sudo usermod -a -G dialout $USER` then log out and back in, or use `sudo python3 send_hil_data.py` |
| Serial port in use | Close any other serial terminals (minicom, screen, Arduino IDE, etc.) |
| Garbled output | Check baud rate matches (default 115200) |
| GUI doesn't open on macOS | Try `python3 -m tkinter` first to verify tkinter is installed |

## Example Workflow

1. **Enable Desktop Mode**: Uncomment `#define DESKTOP_MODE` in app.cpp
2. **Build & Flash**: Rebuild in Simplicity Studio and flash to Gecko
3. **Launch GUI**: `python3 HIL/send_hil_data.py`
4. **Configure**: 
   - Select serial port from dropdown
   - Browse to HIL/48_steps.csv
   - Keep baud rate at 115200
5. **Start**: Click "Start Transmission"
6. **Monitor**: Watch the log for progress and messages
7. **Results**: Final message shows total successful samples sent

## Expected Output

With sample `48_steps.csv`, you should see output like:

```
[HH:MM:SS] [INFO] Connecting to /dev/ttyUSB0 at 115200 baud...
[HH:MM:SS] [OK] Connected to /dev/ttyUSB0
[HH:MM:SS] [INFO] Reading data from: 48_steps.csv
[HH:MM:SS] [INFO] Skipping header: x,y,z
[HH:MM:SS] [INFO] Sending 48 acceleration samples at 10 Hz

[HH:MM:SS] [INFO] → 1,7,112
[HH:MM:SS] [OK] ← Steps: 0  [ACK]
[HH:MM:SS] [INFO] → 1,7,111
[HH:MM:SS] [OK] ← Steps: 0  [ACK]
...
[HH:MM:SS] [INFO] → -6,7,127
[HH:MM:SS] [OK] ← Steps: 48  [ACK]

[HH:MM:SS] [INFO] ==================================================
[HH:MM:SS] [OK] Transmission complete!
[HH:MM:SS] [OK] All 48 samples sent successfully
[HH:MM:SS] [INFO] ==================================================
```

The final step count depends on the algorithm's detection threshold.

## Creating Custom Datasets

Record acceleration data from the ADXL345 sensor and format as CSV (x,y,z format), then load it in the GUI:

1. Prepare your CSV file with columns: x, y, z
2. Launch the GUI: `python3 send_hil_data.py`
3. Browse to your custom file
4. Click "Start Transmission"

## Technical Details

### UARTSerial Protocol (Hardware Side)
The Gecko firmware implements desktop mode using the UARTSerial class:
- Receives CSV data lines from PC
- Processes each sample through the step-detection FSM
- Reports step count: `"Steps: X\n"`
- Sends ACK character: `'+'\n` to signal readiness

### GUI Architecture
- **Main Thread**: Tkinter GUI event loop
- **Worker Thread**: UART communication in background
- **Thread-Safe Updates**: Log and progress bar updated via tkinter's thread-safe methods
- **Blocking Prevention**: Background thread prevents UI freezing

### Error Handling
- Port not found → Error message dialog
- File not found → Error message dialog
- ACK timeout → Error logged, transmission continues
- Serial exception → Caught and displayed, port closed
- User stop → Graceful termination, controls re-enabled

