# Streaming Serial Data from the Gecko Board

Use the **Arduino IDE 1.x Serial Monitor** to view and capture data from the Gecko board. This works the same way on Windows, macOS, and Linux.

---

## Step 1: Install Arduino IDE 1.x

Download **Arduino IDE 1.8.x** from the official site:

**https://www.arduino.cc/en/software**

Scroll down to **"Legacy IDE (1.8.X)"** and download the installer for your operating system. Install it normally.

> **Why version 1?** The Serial Monitor in Arduino 1.x supports Ctrl+A to select all output, making it easy to copy data to a file. Arduino 2.x doesn't have this feature.

---

## Step 2: Connect Your Gecko Board

Plug your Gecko board into your computer via USB. The board should already have the firmware flashed (see [Flash.md](Flash.md)).

---

## Step 3: Select the Correct Port

In Arduino IDE:

1. Go to **Tools** → **Port**
2. Select the port that corresponds to your Gecko board:
   - **Windows:** Look for `COM3`, `COM4`, etc. (try each if unsure)
   - **macOS:** Look for `/dev/cu.usbmodem...`
   - **Linux:** Look for `/dev/ttyUSB0` or `/dev/ttyACM0`

> **Tip:** If you don't see a port, try unplugging and replugging the board, then check again.

---

## Step 4: Open the Serial Monitor

Go to **Tools** → **Serial Monitor** (or press `Ctrl+Shift+M` / `Cmd+Shift+M`).

---

## Step 5: Set the Baud Rate

In the bottom-right corner of the Serial Monitor window, set the baud rate to:

**115200 baud**

Data should begin appearing in the window immediately.

---

## Step 6: Capture the Data

Once you have collected enough data:

1. Click anywhere inside the Serial Monitor output area
2. Press **Ctrl+A** (Windows/Linux) or **Cmd+A** (macOS) to select all text
3. Press **Ctrl+C** / **Cmd+C** to copy
4. Open a text editor (Notepad, TextEdit, gedit, etc.)
5. Paste with **Ctrl+V** / **Cmd+V**
6. Save the file as `data.csv`

---

# Alternative: Using the Gecko Serial GUI Tool

If you prefer a more automated approach, use the **stream_serial_gui.py** tool—a high-performance Python GUI application that automatically streams data with no lag and allows you to record to a file with a single button press.

## Features

- **Auto-streaming:** Data streams live immediately when you connect—no manual start/stop needed
- **High performance:** Buffered reads (4KB chunks) prevent data loss even at 115200 baud
- **One-button recording:** Press "Record" to start saving, press again to stop
- **Automatic filtering:** Control characters are filtered out automatically
- **Live statistics:** See bytes received and bytes saved in real-time
- **Cross-platform:** Works on Windows, macOS, and Linux

## Step 1: Install Python 3

Make sure Python 3 is installed on your system:

**Windows/macOS:**
- Download Python 3 from https://www.python.org/downloads
- Run the installer and follow the prompts

**Linux:**
```bash
sudo apt-get install python3 python3-pip
```

Verify installation:
```bash
python3 --version
```

## Step 2: Install Required Python Packages

Open a terminal/command prompt and run:

```bash
pip install PySimpleGUI pyserial
```

## Step 3: Download the GUI Tool

Download `stream_serial_gui.py` from the HIL folder in this repository.

## Step 4: Run the GUI

Open a terminal/command prompt, navigate to the folder containing `stream_serial_gui.py`, and run:

```bash
python3 stream_serial_gui.py
```

A window should appear with the title "Gecko Serial Data Streamer".

## Step 5: Connect and Stream

1. **Select Port:** Choose your Gecko board from the "Serial Port" dropdown
   - Gecko boards are automatically labeled with `[GECKO]` for easy identification
   - Click **"Refresh Ports"** if your board doesn't appear
2. **Connect:** Click the **"Connect"** button
   - Data starts streaming immediately in the display window
   - Live byte counter shows data being received
3. **Record to File (Optional):**
   - The filename defaults to `data.csv` (you can change it or browse for a different location)
   - Click the **"🔴 Record"** button to start saving data to the file
   - The button changes to **"⏹️ Stop Recording"** and a red banner shows which file is being recorded to
   - Live byte counters show both received and saved data
   - Click **"⏹️ Stop Recording"** to finish—a popup confirms how many bytes were saved
4. *Zero lag:** Buffered reads (4KB chunks) with separate threads prevent data loss
- **Auto-streaming:** Data displays immediately when you connect—no extra buttons to click
- **Simple recording:** One button to start, same button to stop and save
- **Automatic filtering:** Control characters and escape sequences are filtered out
- **Live preview:** See the data streaming in real-time while it's being saved
- **Performance monitoring:** Live byte counters show exactly how much data has been received and saved
- **Cross-platform:** Works identically on Windows, macOS, and Linux
- **Hands-free operation:** Once connected, just press Record when ready—your board controls the data flow
## Why Use the GUI Tool?

- **Automatic filtering:** Control characters and escape sequences are filtered out automatically
- **Live preview:** See the data streaming in real-time while it's being saved
- **One-click save:** No need to manually select, copy, and paste
- **Cross-platform:** Works on Windows, macOS, and Linux the same way
- **Hands-off:** Once started, data is saved automatically—no manual selection needed

---

## Troubleshooting

**No port visible under Tools → Port**
- Make sure the board is connected via USB
- Try a different cable (some USB cables are power-only)
- Restart Arduino IDE after plugging in the board

**Garbage characters or no data in Serial Monitor**
- Confirm the baud rate is set to **115200**
- Make sure the correct port is selected
- Try pressing the reset button on the Gecko board

**"Permission denied" on Linux**
- Run the following, then log out and back in:
  ```bash
  sudo usermod -a -G dialout $USER
  ```

---

## GUI Tool Troubleshooting

**"PySimpleGUI not installed" error**
- Install it with:
  ```bash
  pip install PySimpleGUI
  ```
Click the **"Refresh Ports"** button
- Try unplugging and replugging the board
- Restart the GUI tool if necessary

**No data appearing in the output window**
- Make sure you clicked **"Connect"** first (data streams automatically after connecting)
- Verify your board is actually sending data (check firmware)
- Try pressing the reset button on the board
- Confirm the board is configured for 115200 baud

**Recording button doesn't work**
- You must be **connected** to a serial port first
- If the button is greyed out, click **"Connect"** first

**Data appears in the GUI but file is empty**
- Make sure you clicked the **"🔴 Record"** button (not just Connect)
- The file is only written when recording is active (red banner shows "RECORDING TO FILE")
- Click **"⏹️ Stop Recording"** to flush and close the file
- Check that you have write permissions to the folder

**Screen updates are laggy or data seems slow**
- The GUI is already optimized with 4KB buffered reads
- If you still see issues, your computer may be under heavy load
- The file will still capture all data even if the display lags (file writes have priority)

**"ModuleNotFoundError: No module named 'serial'"**
- Install pyserial with:
  ```bash
  pip install pyserial
  ```

**Recording stopped but I can't find my file**
- By default, files are saved in the same directory where you ran the script
- Check the working directory with `pwd` (macOS/Linux) or `cd` (Windows)
- Use the **"Browse"** button before recording to choose a specific locationtall pyserial with:
  ```bash
  pip install pyserial
  ```
