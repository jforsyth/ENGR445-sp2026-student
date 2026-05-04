# Streaming and Capturing Serial Data from the Gecko Board

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

If you prefer a more automated approach, use the **stream_serial_gui.py** tool—a Python GUI application that automatically filters out control characters and streams clean data to a file.

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

1. **Select Port:** Choose your Gecko board from the "Serial Port" dropdown (it will be labeled `[GECKO]` if detected automatically)
2. **Connect:** Click the **"Connect"** button
3. **Specify File:** Enter a filename (e.g., `data.csv`) in the text field
4. **Save Data:** Click **"Save Data"** to start writing to the file
5. **Start Streaming:** Click the **"Start"** button to begin capturing serial data
6. The output appears in real-time in the display window **and** is saved to your file
7. **Stop:** Click **"Stop"** to end streaming

The GUI automatically filters out control characters, so the saved file contains only clean, readable text.

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

**No ports showing in the GUI dropdown**
- Make sure the board is connected via USB
- Try unplugging and replugging the board
- Restart the GUI tool

**No data appearing in the output window**
- Verify the baud rate is set to **115200** on the Gecko board firmware
- Try pressing the reset button on the board
- Check that the correct port is selected

**File is empty or incomplete**
- Make sure you clicked **"Save Data"** before clicking **"Start"**
- Keep the GUI running long enough to collect data
- Check that you have write permissions to the folder

**"ModuleNotFoundError: No module named 'serial'"**
- Install pyserial with:
  ```bash
  pip install pyserial
  ```
