#!/usr/bin/env python3
"""
Gecko Serial Data Streamer with GUI
Allows students to:
- Select and open serial port (with auto-detection)
- Stream data in real-time (auto-starts on connect)
- Save streamed data to a file

Performance optimizations:
- Buffered reads (4KB chunks)
- Separate threads for data collection and GUI updates
- Queue-based architecture to prevent data loss
"""

import PySimpleGUI as sg
from serial import Serial
import serial.tools.list_ports
import threading
import time
import queue
from datetime import datetime
from pathlib import Path

# Settings (from hil-app.py pattern)
BAUD_RATE = 115200
READ_TIMEOUT = 0.1  # Shorter timeout for responsiveness
GECKO_IDENTIFIERS = ['usbmodem', 'Silicon']

# Performance settings
READ_BUFFER_SIZE = 4096  # Read 4KB at a time for efficiency
GUI_UPDATE_INTERVAL = 0.05  # Update GUI every 50ms (20 Hz)
QUEUE_MAX_SIZE = 1000  # Queue size to buffer data between threads

# GUI Theme
sg.theme('DarkBlue3')

class SerialStreamer:
    def __init__(self):
        self.ser = None
        self.is_running = False
        self.is_streaming = False
        self.save_file = None
        self.file_handle = None
        self.data_queue = queue.Queue(maxsize=QUEUE_MAX_SIZE)
        self.bytes_written = 0
        self.bytes_received = 0
        
    def find_gecko_ports(self):
        """Find all available ports, highlight Gecko if found"""
        try:
            ports = serial.tools.list_ports.comports()
            port_list = []
            gecko_port = None
            
            if not ports:
                return ["No ports found"], None
            
            for port in ports:
                try:
                    device_str = str(port.device).lower()
                    desc_str = str(port.description).lower()
                    
                    # Check if it's a Gecko
                    is_gecko = any(id.lower() in device_str or id.lower() in desc_str 
                                  for id in GECKO_IDENTIFIERS)
                    
                    display_name = f"{port.device} - {port.description}"
                    if is_gecko:
                        display_name += " [GECKO]"
                        if gecko_port is None:
                            gecko_port = port.device
                    
                    port_list.append(display_name)
                except Exception as e:
                    print(f"Error processing port: {e}")
                    continue
            
            return port_list if port_list else ["No ports found"], gecko_port
        except Exception as e:
            print(f"Error finding ports: {e}")
            return ["Error reading ports"], None
    
    def connect(self, port_name):
        """Connect to serial port"""
        try:
            if not port_name or port_name == "No ports found":
                return False, "Please select a valid port"
            
            # Extract just the port device path (before " - ")
            port_device = port_name.split(" - ")[0]
            
            # Validate port_device
            if not port_device or port_device.strip() == "":
                return False, "Invalid port selection"
            
            self.ser = Serial(port_device, BAUD_RATE, timeout=READ_TIMEOUT)
            time.sleep(0.5)
            return True, f"Connected to {port_device} at {BAUD_RATE} baud"
        except AttributeError as e:
            return False, f"Serial module error: {e}"
        except Exception as e:
            return False, f"Failed to connect: {e}"
    
    def disconnect(self):
        """Disconnect from serial port"""
        if self.ser:
            self.ser.close()
            self.ser = None
        self.is_streaming = False
        if self.file_handle:
            self.file_handle.close()
            self.file_handle = None
    
    def set_save_file(self, file_path):
        """Set file to save data to"""
        try:
            self.save_file = file_path
            self.file_handle = open(file_path, 'w', buffering=8192)  # 8KB buffer
            self.bytes_written = 0
            return True, f"📝 Recording to: {Path(file_path).name}"
        except Exception as e:
            return False, f"Failed to open file: {e}"
    
    def stop_saving(self):
        """Close the save file"""
        if self.file_handle:
            self.file_handle.flush()
            self.file_handle.close()
            self.file_handle = None
            saved_file = self.save_file
            self.save_file = None
            return saved_file
        return None
    
    def read_data_chunk(self):
        """Read chunk of data from serial port (called by high-priority thread)"""
        try:
            if not self.ser or not self.ser.is_open:
                return None
            
            # Read up to READ_BUFFER_SIZE bytes
            data = self.ser.read(READ_BUFFER_SIZE)
            if data:
                self.bytes_received += len(data)
                text = data.decode('utf-8', errors='replace')
                
                # Filter control characters but keep newlines/returns
                filtered = ''.join(char for char in text 
                                 if ord(char) >= 32 or char in '\n\r')
                
                # Immediately save to file if open (high priority)
                if self.file_handle and filtered:
                    self.file_handle.write(filtered)
                    self.bytes_written += len(filtered)
                    # Flush every ~8KB for balance of performance and safety
                    if self.bytes_written % 8192 < len(filtered):
                        self.file_handle.flush()
                
                return filtered if filtered else None
        except Exception as e:
            print(f"Error reading serial data: {e}")
        
        return None

def main():
    streamer = SerialStreamer()
    
    # Get initial port list
    port_list, gecko_port = streamer.find_gecko_ports()
    default_port = gecko_port if gecko_port else (port_list[0] if port_list else "")
    
    # Define the layout
    layout = [
        [sg.Text("Gecko Serial Data Streamer", font=("Helvetica", 16, "bold"))],
        [sg.Text("_" * 60)],
        
        # Port selection
        [
            sg.Text("Serial Port:", size=(12, 1)),
            sg.Combo(port_list, default_value=default_port, 
                    key="-PORT-", size=(35, 1), readonly=True),
            sg.Button("Refresh Ports", size=(12, 1))
        ],
        
        # Connection status
        [sg.Text("Status:", size=(12, 1)), 
         sg.Text("Not connected", key="-STATUS-", size=(45, 1), 
                text_color="orange")],
        
        # Connect/Disconnect buttons
        [
            sg.Button("Connect", size=(12, 1), key="-CONNECT-"),
            sg.Button("Disconnect", size=(12, 1), key="-DISCONNECT-", disabled=True),
            sg.VerticalSeparator(),
            sg.Button("Refresh Ports", size=(12, 1)),
        ],
        
        [sg.Text("_" * 60)],
        
        # File recording controls
        [
            sg.Text("Filename:", size=(12, 1)),
            sg.InputText("data.csv", key="-FILEPATH-", size=(30, 1), disabled=True),
            sg.Button("Browse", size=(10, 1), key="-BROWSE-", disabled=True),
            sg.Button("🔴 Record", size=(14, 1), key="-RECORD-", disabled=True, button_color=('white', 'red')),
        ],
        
        # File recording indicator
        [
            sg.Text("", key="-RECORDING-", size=(60, 1), 
                   font=("Helvetica", 12, "bold"), text_color="red", visible=False)
        ],
        
        # Stats
        [
            sg.Text("Received: 0 bytes", key="-STATS-", size=(30, 1), text_color="cyan"),
            sg.Text("Saved: 0 bytes", key="-SAVED-", size=(30, 1), text_color="lime", visible=False)
        ],
        
        [sg.Text("_" * 60)],
        
        # Data display area
        [sg.Text("Serial Data Output:")],
        [sg.Multiline(size=(80, 18), key="-OUTPUT-", 
                     disabled=True, autoscroll=True)],
        
        # Clear button
        [sg.Button("Clear Output", size=(12, 1)), 
         sg.Button("Exit", size=(12, 1))],
    ]
    
    # Create window
    window = sg.Window("Gecko Serial Data Streamer", layout, finalize=True)
    
    # Threading variables
    read_thread = None
    gui_update_thread = None
    
    def read_thread_func():
        """High-priority thread: read serial data and write to file immediately"""
        while streamer.is_streaming and streamer.ser and streamer.ser.is_open:
            chunk = streamer.read_data_chunk()
            if chunk:
                # Try to queue for GUI (non-blocking)
                try:
                    streamer.data_queue.put_nowait(chunk)
                except queue.Full:
                    # Queue full - data still saved to file, just won't show in GUI
                    print("GUI queue full - some display data dropped (file OK)")
            else:
                # No data available, small sleep to prevent CPU spinning
                time.sleep(0.01)
    
    def gui_update_thread_func():
        """Lower-priority thread: update GUI with queued data"""
        accumulated = []
        last_update = time.time()
        
        while streamer.is_streaming:
            try:
                # Collect data from queue
                while True:
                    try:
                        chunk = streamer.data_queue.get_nowait()
                        accumulated.append(chunk)
                    except queue.Empty:
                        break
                
                # Update GUI periodically
                now = time.time()
                if accumulated and (now - last_update >= GUI_UPDATE_INTERVAL):
                    text = ''.join(accumulated)
                    window["-OUTPUT-"].print(text, end='')
                    accumulated.clear()
                    last_update = now
                
                time.sleep(0.01)  # Small sleep
                
            except Exception as e:
                print(f"GUI update error: {e}")
                time.sleep(0.1)
        
        # Flush remaining data
        if accumulated:
            text = ''.join(accumulated)
            window["-OUTPUT-"].print(text, end='')
    
    # Main event loop
    try:
        while True:
            event, values = window.read(timeout=100)
            
            if event == sg.WINDOW_CLOSED or event == "Exit":
                break
            
            elif event == "Refresh Ports":
                port_list, gecko_port = streamer.find_gecko_ports()
                if port_list:
                    window["-PORT-"].update(values=port_list, 
                                           value=gecko_port if gecko_port else port_list[0])
                else:
                    window["-PORT-"].update(values=["No ports found"], value="No ports found")
            
            elif event == "-CONNECT-":
                if not values["-PORT-"]:
                    sg.popup_error("Please select a port first!")
                    continue
                
                success, msg = streamer.connect(values["-PORT-"])
                if success:
                    # Auto-start streaming when connected
                    streamer.is_streaming = True
                    streamer.bytes_received = 0
                    
                    window["-STATUS-"].update(msg + " - Live streaming...", text_color="green")
                    window["-CONNECT-"].update(disabled=True)
                    window["-DISCONNECT-"].update(disabled=False)
                    window["-PORT-"].update(disabled=True)
                    window["-RECORD-"].update(disabled=False)
                    window["-BROWSE-"].update(disabled=False)
                    window["-FILEPATH-"].update(disabled=False)
                    window["-STATS-"].update("Received: 0 bytes")
                    
                    # Start both threads
                    read_thread = threading.Thread(target=read_thread_func, daemon=True)
                    gui_update_thread = threading.Thread(target=gui_update_thread_func, daemon=True)
                    read_thread.start()
                    gui_update_thread.start()
                else:
                    window["-STATUS-"].update(msg, text_color="red")
            
            elif event == "-DISCONNECT-":
                # Stop streaming
                streamer.is_streaming = False
                if read_thread:
                    read_thread.join(timeout=1)
                if gui_update_thread:
                    gui_update_thread.join(timeout=1)
                
                # Stop file saving if active
                saved_file = streamer.stop_saving()
                if saved_file:
                    sg.popup(f"✅ Saved {streamer.bytes_written:,} bytes to {Path(saved_file).name}", 
                            title="Recording Saved", keep_on_top=True)
                
                streamer.disconnect()
                window["-STATUS-"].update("Not connected", text_color="orange")
                window["-CONNECT-"].update(disabled=False)
                window["-DISCONNECT-"].update(disabled=True)
                window["-RECORD-"].update("🔴 Record", disabled=True, button_color=('white', 'red'))
                window["-BROWSE-"].update(disabled=True)
                window["-FILEPATH-"].update(disabled=True, value="data.csv")
                window["-PORT-"].update(disabled=False)
                window["-RECORDING-"].update(visible=False)
                window["-SAVED-"].update(visible=False)
                window["-STATS-"].update("Received: 0 bytes")
            
            elif event == "-BROWSE-":
                # Open file browser dialog
                file_path = sg.popup_get_file(
                    "Select file to save data",
                    save_as=True,
                    file_types=(("CSV files", "*.csv"), ("Text files", "*.txt"), ("All files", "*.*")),
                    default_extension=".csv"
                )
                if file_path:
                    window["-FILEPATH-"].update(file_path)
            
            elif event == "-RECORD-":
                if not streamer.is_streaming:
                    sg.popup_error("Not connected! Connect to serial port first.")
                    continue
                
                # Toggle recording
                if not streamer.file_handle:
                    # Start recording
                    filepath = values["-FILEPATH-"].strip()
                    
                    # Default to data.csv if empty
                    if not filepath:
                        filepath = "data.csv"
                    
                    # Add .csv extension if no extension provided
                    if "." not in filepath.split("/")[-1].split("\\")[-1]:
                        filepath = filepath + ".csv"
                    
                    success, msg = streamer.set_save_file(filepath)
                    if success:
                        window["-STATUS-"].update(msg, text_color="green")
                        window["-RECORDING-"].update(f"🔴 RECORDING TO FILE: {Path(filepath).name}", 
                                                     visible=True)
                        window["-SAVED-"].update("Saved: 0 bytes", visible=True)
                        window["-RECORD-"].update("⏹️ Stop Recording", button_color=('white', 'darkred'))
                        window["-BROWSE-"].update(disabled=True)
                        window["-FILEPATH-"].update(disabled=True)
                    else:
                        window["-STATUS-"].update(msg, text_color="red")
                else:
                    # Stop recording
                    saved_file = streamer.stop_saving()
                    if saved_file:
                        sg.popup(f"✅ Saved {streamer.bytes_written:,} bytes to {Path(saved_file).name}", 
                                title="Recording Saved", keep_on_top=True)
                        window["-STATUS-"].update(f"Recording stopped - {streamer.bytes_written:,} bytes saved", 
                                                 text_color="green")
                    window["-RECORDING-"].update(visible=False)
                    window["-SAVED-"].update(visible=False)
                    window["-RECORD-"].update("🔴 Record", button_color=('white', 'red'))
                    window["-BROWSE-"].update(disabled=False)
                    window["-FILEPATH-"].update(disabled=False)
            
            elif event == "Clear Output":
                window["-OUTPUT-"].update("")
            
            # Update stats periodically (every event loop iteration)
            if streamer.is_streaming:
                window["-STATS-"].update(f"Received: {streamer.bytes_received:,} bytes")
                if streamer.file_handle:
                    window["-SAVED-"].update(f"Saved: {streamer.bytes_written:,} bytes")
    
    except Exception as e:
        sg.popup_error(f"Error: {e}")
    
    finally:
        streamer.disconnect()
        window.close()

if __name__ == "__main__":
    try:
        main()
    except ImportError:
        sg.popup_error(
            "PySimpleGUI not installed!\n\n"
            "Install with:\n"
            "pip install PySimpleGUI"
        )
