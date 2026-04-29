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
    
    inline bool init_csv_reader(const char* filepath)
    {
        fin.open(std::string("../") + filepath);
        if(fin.fail()) {
            std::cout << "Failed opening file" << std::endl;
            return false;
        }
        std::string header;
        std::getline(fin, header);
        std::cout << "Header line skipped: " << header << std::endl;
        return true;
    }
    
    inline bool read_csv_sample(AccelSample* sample)
    {
        std::string line;
        if(!std::getline(fin, line)) {
            return false;
        }
        std::vector<std::string> parts;
        if(!splits(line, parts)) {
            return false;
        }
        if(parts.size() < 4) {
            return false;
        }
        try {
            sample->timestamp_ms = (uint32_t)(std::stof(parts[0]) * 1000.0f);
            sample->x = std::stof(parts[1]);
            sample->y = std::stof(parts[2]);
            sample->z = std::stof(parts[3]);
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