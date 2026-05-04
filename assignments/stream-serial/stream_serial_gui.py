#!/usr/bin/env python3
"""
Gecko Serial Data Streamer with GUI
Allows students to:
- Select and open serial port (with auto-detection)
- Stream data in real-time
- Save streamed data to a file
"""

import PySimpleGUI as sg
from serial import Serial
import serial.tools.list_ports
import threading
import time
from datetime import datetime
from pathlib import Path

# Settings (from hil-app.py pattern)
BAUD_RATE = 115200
READ_TIMEOUT = 1  # Shorter timeout for GUI responsiveness
GECKO_IDENTIFIERS = ['usbmodem', 'Silicon']

# GUI Theme
sg.theme('DarkBlue3')

class SerialStreamer:
    def __init__(self):
        self.ser = None
        self.is_running = False
        self.is_streaming = False
        self.save_file = None
        self.file_handle = None
        self.data_buffer = ""
        
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
            self.file_handle = open(file_path, 'w')
            return True, f"Saving to: {file_path}"
        except Exception as e:
            return False, f"Failed to open file: {e}"
    
    def stop_saving(self):
        """Close the save file"""
        if self.file_handle:
            self.file_handle.close()
            self.file_handle = None
            self.save_file = None
    
    def read_data(self):
        """Read data from serial port (called by thread)"""
        try:
            if not self.ser or not self.ser.is_open:
                return None
            
            data = self.ser.read(1)
            if data:
                char = data.decode('utf-8', errors='replace')
                
                # Filter out control characters, keep printable ASCII (32-126) and line breaks
                if ord(char) >= 32 or char in '\n\r':
                    # Optionally save to file
                    if self.file_handle:
                        self.file_handle.write(char)
                        self.file_handle.flush()
                    
                    return char
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
        
        # Start/Stop streaming
        [
            sg.Text("Streaming:", size=(12, 1)),
            sg.Button("Start", size=(12, 1), key="-START-", disabled=True),
            sg.Button("Stop", size=(12, 1), key="-STOP-", disabled=True),
            sg.VerticalSeparator(),
            sg.Text("Filename:", size=(10, 1)),
            sg.InputText(key="-FILEPATH-", size=(22, 1), disabled=True),
            sg.Button("Browse", size=(8, 1), key="-BROWSE-", disabled=True),
            sg.Button("Save Data", size=(10, 1), key="-SAVE-", disabled=True),
        ],
        
        [sg.Text("_" * 60)],
        
        # Data display area
        [sg.Text("Serial Data Output:")],
        [sg.Multiline(size=(80, 20), key="-OUTPUT-", 
                     disabled=True, autoscroll=True)],
        
        # Clear button
        [sg.Button("Clear Output", size=(12, 1)), 
         sg.Button("Exit", size=(12, 1))],
    ]
    
    # Create window
    window = sg.Window("Gecko Serial Data Streamer", layout, finalize=True)
    
    # Threading variables
    read_thread = None
    
    def read_thread_func():
        """Thread function to read serial data"""
        while streamer.is_streaming and streamer.ser and streamer.ser.is_open:
            char = streamer.read_data()
            if char:
                window["-OUTPUT-"].print(char, end='')
            time.sleep(0.001)  # Small delay to prevent CPU spinning
    
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
                    window["-STATUS-"].update(msg, text_color="green")
                    window["-CONNECT-"].update(disabled=True)
                    window["-DISCONNECT-"].update(disabled=False)
                    window["-START-"].update(disabled=False)
                    window["-PORT-"].update(disabled=True)
                else:
                    window["-STATUS-"].update(msg, text_color="red")
            
            elif event == "-DISCONNECT-":
                streamer.disconnect()
                window["-STATUS-"].update("Not connected", text_color="orange")
                window["-CONNECT-"].update(disabled=False)
                window["-DISCONNECT-"].update(disabled=True)
                window["-START-"].update(disabled=True)
                window["-STOP-"].update(disabled=True)
                window["-SAVE-"].update(disabled=True)
                window["-BROWSE-"].update(disabled=True)
                window["-FILEPATH-"].update(disabled=True, value="")
                window["-PORT-"].update(disabled=False)
                streamer.stop_saving()
            
            elif event == "-START-":
                if not streamer.ser or not streamer.ser.is_open:
                    sg.popup_error("Not connected!")
                    continue
                
                streamer.is_streaming = True
                window["-START-"].update(disabled=True)
                window["-STOP-"].update(disabled=False)
                window["-SAVE-"].update(disabled=False)
                window["-BROWSE-"].update(disabled=False)
                window["-FILEPATH-"].update(disabled=False)
                
                # Start read thread
                read_thread = threading.Thread(target=read_thread_func, daemon=True)
                read_thread.start()
            
            elif event == "-STOP-":
                streamer.is_streaming = False
                if read_thread:
                    read_thread.join(timeout=1)
                window["-START-"].update(disabled=False)
                window["-STOP-"].update(disabled=True)
            
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
            
            elif event == "-SAVE-":
                filepath = values["-FILEPATH-"].strip()
                
                if not filepath:
                    sg.popup_error("Please enter or select a filename!")
                    continue
                
                if not streamer.is_streaming:
                    sg.popup_error("Start streaming first!")
                    continue
                
                # Add .csv extension if no extension provided
                if "." not in filepath.split("/")[-1].split("\\")[-1]:
                    filepath = filepath + ".csv"
                
                success, msg = streamer.set_save_file(filepath)
                if success:
                    window["-STATUS-"].update(msg, text_color="green")
                    window["-SAVE-"].update(disabled=True)
                    window["-BROWSE-"].update(disabled=True)
                    window["-FILEPATH-"].update(disabled=True)
                else:
                    window["-STATUS-"].update(msg, text_color="red")
            
            elif event == "Clear Output":
                window["-OUTPUT-"].update("")
    
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
