//
// Created by Jason Forsyth on 10/27/24.
//
#ifndef APP_H
#define APP_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

/***************************************************************************//**
 * CONFIGURATION: Build Target Selection
 * Toggle this to switch between desktop (CSV) and embedded (real accelerometer)
 ******************************************************************************/
#define DESKTOP_BUILD  // Comment out this line for Giant Gecko build

/***************************************************************************//**
 * Accelerometer Sample Structure
 * This is the data format used internally, regardless of source (CSV or hardware)
 ******************************************************************************/
struct AccelSample {
    float x;              // X-axis acceleration
    float y;              // Y-axis acceleration
    float z;              // Z-axis acceleration
    uint32_t timestamp_ms; // Timestamp in milliseconds
};

/***************************************************************************//**
 * Initialize application.
 * Called once by main() at startup.
 * - Initializes CSV reader or hardware accelerometer
 * - Sets up state machine and data structures
 * @return true if initialization succeeds, false on error
 ******************************************************************************/
bool app_init(void);

/***************************************************************************//**
 * App ticking function (called repeatedly by main()).
 * This is where you implement your step-counting algorithm.
 * Called ~100 times per second (one per sample) by main.cpp until it returns false.
 * @return true to continue processing, false when done (e.g., end of data)
 ******************************************************************************/
bool app_process_action(void);

/***************************************************************************//**
 * Abstraction layer: Get the next acceleration sample.
 * Routes to either CSV (desktop) or ADXL345 (embedded) based on build configuration.
 ******************************************************************************/
bool get_next_sample(AccelSample* sample);

/**
 * Utility: Parse comma-separated values from a string
 * This is infrastructure code - students don't need to implement this.
 *
 * @param line The CSV line to be parsed
 * @param container A vector<string> container to hold the results
 * @return Return true if parsing succeeds, false otherwise
 */
inline bool splits(std::string line, std::vector<std::string>& container)
{
    std::vector<std::string>& parts = container;
    int start_pos = 0;
    int end_pos = -1;

    while(start_pos != -1)
    {
        end_pos = line.find_first_of(',');
        if (end_pos == -1 && !line.empty())
        {
            parts.push_back(line);
            return true;
        }
        std::string sub_string = line.substr(start_pos, end_pos);
        parts.push_back(sub_string);
        line = line.substr(end_pos + 1, line.length() - 1);

        if(line.empty())
        {
            return true;
        }
    }

    return false;
}

#ifdef DESKTOP_BUILD
    extern const char* DEFAULT_CSV_PATH;
    extern std::ifstream fin;

    // Global variables to store column indices
    static int time_idx = -1;
    static int accel_x_idx = -1;
    static int accel_y_idx = -1;
    static int accel_z_idx = -1;
    static std::string buffered_line;
    static bool has_buffered_line = false;

    inline bool init_csv_reader(const char* filepath)
    {
        // Use absolute paths as-is; prepend "../" for relative paths so the
        // binary running from cmake-build-debug/ can find files in the project root.
        // Absolute path detection:
        //   Unix/macOS: starts with '/'
        //   Windows:    starts with a drive letter + ':' (e.g. "C:\") or a UNC path "\\"
        std::string fp(filepath);
        bool is_absolute = !fp.empty() && (
            fp[0] == '/' ||                          // Unix / macOS
            fp[0] == '\\' ||                         // Windows UNC path
            (fp.size() >= 2 && fp[1] == ':')         // Windows drive letter (C:, D:, etc.)
        );
        std::string path = is_absolute ? fp : std::string("../") + fp;
        fin.open(path);
        if(fin.fail()) {
            std::cout << "Failed opening file" << std::endl;
            return false;
        }

        // Reset buffered line state
        has_buffered_line = false;
        buffered_line.clear();

        std::string first_line;
        std::getline(fin, first_line);
        std::cout << "First line read: " << first_line << std::endl;

        // Parse first line
        std::vector<std::string> parts;
        if(!splits(first_line, parts)) {
            std::cout << "Failed to parse first line" << std::endl;
            return false;
        }

        // Try to detect if this is a header or data
        // If first column can be parsed as a number, assume it's data (no header)
        bool is_header = false;
        try {
            float first_val = std::stof(parts[0]);
            // If we got here, it parsed as a number - probably data, not a header
            is_header = false;
        } catch (...) {
            // Failed to parse as number - probably a header
            is_header = true;
        }

        if(is_header) {
            // Parse header to find column indices
            time_idx = -1;
            accel_x_idx = -1;
            accel_y_idx = -1;
            accel_z_idx = -1;

            for(int i = 0; i < parts.size(); i++) {
                std::string col = parts[i];
                if(col == "time" || col == "Time" || col == "TIME") {
                    time_idx = i;
                }
                else if(col == "accel_x" || col == "accel_x" || col == "ACCEL_X" || col == "ax" || col == "AX") {
                    accel_x_idx = i;
                }
                else if(col == "accel_y" || col == "accel_y" || col == "ACCEL_Y" || col == "ay" || col == "AY") {
                    accel_y_idx = i;
                }
                else if(col == "accel_z" || col == "accel_z" || col == "ACCEL_Z" || col == "az" || col == "AZ") {
                    accel_z_idx = i;
                }
            }

            // Validate that all required columns were found
            if(time_idx == -1 || accel_x_idx == -1 || accel_y_idx == -1 || accel_z_idx == -1) {
                std::cout << "Error: Could not find all required columns (time, accel_x, accel_y, accel_z)" << std::endl;
                std::cout << "Found indices: time=" << time_idx << ", accel_x=" << accel_x_idx
                          << ", accel_y=" << accel_y_idx << ", accel_z=" << accel_z_idx << std::endl;
                return false;
            }

            std::cout << "Header detected. Column indices: time=" << time_idx << ", accel_x=" << accel_x_idx
                      << ", accel_y=" << accel_y_idx << ", accel_z=" << accel_z_idx << std::endl;
        } else {
            // No header - assume defaults: time, ax, ay, az
            std::cout << "No header detected. Assuming columns: time, ax, ay, az" << std::endl;
            time_idx = 0;
            accel_x_idx = 1;
            accel_y_idx = 2;
            accel_z_idx = 3;

            // Buffer the first line since it's actually data
            buffered_line = first_line;
            has_buffered_line = true;

            std::cout << "Column indices: time=" << time_idx << ", accel_x=" << accel_x_idx
                      << ", accel_y=" << accel_y_idx << ", accel_z=" << accel_z_idx << std::endl;
        }

        return true;
    }

    inline bool read_csv_sample(AccelSample* sample)
    {
        std::string line;

        // Check if we have a buffered line from header detection
        if(has_buffered_line) {
            line = buffered_line;
            has_buffered_line = false;
        } else {
            if(!std::getline(fin, line)) {
                return false;
            }
        }

        std::vector<std::string> parts;
        if(!splits(line, parts)) {
            return false;
        }

        // Check that we have enough columns
        int max_idx = std::max({time_idx, accel_x_idx, accel_y_idx, accel_z_idx});
        if(parts.size() <= max_idx) {
            return false;
        }

        try {
            sample->timestamp_ms = (uint32_t)(std::stof(parts[time_idx]) * 1000.0f);
            sample->x = std::stof(parts[accel_x_idx]);
            sample->y = std::stof(parts[accel_y_idx]);
            sample->z = std::stof(parts[accel_z_idx]);
            return true;
        } catch (...) {
            return false;
        }
    }
#else
    // TODO: Implement Giant Gecko ADXL345-specific functions
    // These should handle I2C communication with the accelerometer
    bool init_adxl345(void);
    bool read_adxl345_sample(AccelSample* sample);
#endif

ifstream load_csv_file(string filepath);
#endif  // APP_H