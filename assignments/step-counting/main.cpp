#include <iostream>
#include "App.h"
using namespace std;

/**
 * MAIN EXECUTION FLOW
 * 
 * This mirrors the Simplicity Studio embedded systems pattern:
 * 
 *   1. Call app_init() once
 *      - Initialize state machine, data structures, and data source (CSV or ADXL345)
 *      - Returns true on success, false on error
 * 
 *   2. Call app_process_action() in a loop
 *      - Processes one acceleration sample per call (~100 Hz)
 *      - Returns true to continue looping
 *      - Returns false when done (e.g., end of file or error)
 * 
 *   3. Exit when app_process_action() returns false
 * 
 * YOUR IMPLEMENTATION:
 * - Do not modify main.cpp
 * - Implement your algorithm in app_process_action() in App.cpp
 * - Use app_init() to set up initial state
 */

int main() {
    cout << "Booting up your Program...Calling init() function" << endl;

    bool init_success = app_init();

    if (!init_success) {
        cout << "Initialization failed. Program will not proceed. Exiting." << endl;
        return -1;
    }

    cout << "Initialization success. Will now proceed to called your process_action() function repeatedly.." << endl;
    int LOOP_ACTION_LIMIT = 1000000;
    for (int i = 0; i < LOOP_ACTION_LIMIT; i++) {
        bool success = app_process_action();

        if (!success) {
            cout << "App action exiting..." << endl;
            return -1;
        }
    }

    cout << "This should never happen. main() has called your code " << LOOP_ACTION_LIMIT << " times." << endl;

    return -1;
}