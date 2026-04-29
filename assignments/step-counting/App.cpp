#include "App.h"
using namespace std;

/**
 * APPLICATION ARCHITECTURE
 * 
 * This file implements the core application structure that mirrors the Simplicity Studio
 * embedded systems pattern:
 * 
 *   1. main.cpp calls app_init() once at startup
 *      → Initializes the CSV reader, state machine, and all global data structures
 *      → Returns true if initialization succeeds, false on error
 * 
 *   2. main.cpp then calls app_process_action() repeatedly in a loop
 *      → Runs once per acceleration sample (~100 Hz)
 *      → Returns true to continue looping
 *      → Returns false when done (e.g., end of data file)
 * 
 *   3. The loop terminates when app_process_action() returns false
 * 
 * STUDENT IMPLEMENTATION:
 * - Implement your step-detection state machine in app_process_action()
 * - Use the global arrays (magnitude, diffs, squares, averages) as needed
 * - Modify the STATE enum and nextState logic to detect walking periods
 * - Use get_next_sample() to acquire acceleration data
 * 
 * DATA FLOW:
 * - get_next_sample() abstracts the data source (CSV file on desktop, ADXL345 on embedded)
 * - Each sample contains x, y, z acceleration and a timestamp
 * - Your algorithm processes samples one at a time through the state machine
 */

/**
 * File to read: Change this to test different datasets
 * Available files:
 *   - p1.1_Female_20-29_170-179cm_Hand_held.out.csv (70 steps)
 *   - p1.4_Female_20-29_170-179cm_Handbag.out.csv (70 steps)
 *   - p2.2_Male_20-29_180-189cm_Hand_held.out.csv (66 steps)
 *   - p9.2_Female_15-19_160-169cm_Trousers_back_pocket.out.csv (70 steps)
 *   - p11.3_Male_20-29_170-179cm_Backpack.out.csv (76 steps)
 *   - p27.1_Male_15-19_170-179cm_Hand_held.dat.csv (68 steps)
 */
const char* DEFAULT_CSV_PATH = "p27.1_Male_15-19_170-179cm_Hand_held.dat.csv";

/**
 * Create a file input stream that we will constantly read from.
 */
ifstream fin;


/**
 * Define all global variables and data arrays
 */
const int ARRAY_LENGTH = 16;
float magnitude[ARRAY_LENGTH];
float diffs[ARRAY_LENGTH];
float squares[ARRAY_LENGTH];
float averages[ARRAY_LENGTH];

/**
 * Enumerate some basic states the machine may wish to follow
 */
enum STATE {IDLE, DETECT, TIMEOUT};
STATE currentState;
STATE nextState;


bool app_init(void)
{
    /**
     * Step 0: Initial all arrays defined above to known values. Really should be 0.
     * Will use FOR LOOP that is very inefficient but is guaranteed to work on all systems
     */

    for(int i=0;i<ARRAY_LENGTH;i++)
    {
        /***
         * Your code here
         */
    }

    // Initialize the states of the machine to the initial one
    currentState = IDLE;
    nextState = IDLE;

    cout << "Initializing CSV reader..." << endl;
    if(!init_csv_reader(DEFAULT_CSV_PATH)) {
        return false;
    }

    /**
     * Step 2: Initialize any other system level operations: Timers, LCD screen...etc
     */
    // do nothing at this time

    return true;
}

/**
 * Create a counter variable to track how ofter this loop is called. On the PC, this will correspond to each line
 * of the text file. On the Giant Gecko, this will be each "tick" of the program at each sample.
 */
bool app_process_action(void)
{
    static int idx = 0;

    AccelSample sample;
    if(!get_next_sample(&sample))
    {
        cout << "No more samples available. Exiting." << endl;
        return false;
    }

    // Extract acceleration values from the unified sample structure
    float x = sample.x;
    float y = sample.y;
    float z = sample.z;
    uint32_t timestamp_ms = sample.timestamp_ms;

    /**
     * Step 2: Begin data processing pipeline. The first action is likely to calculate the magnitude
     * of the most recent sample.
     */

    /**
     * Your code here
     */

    /**
     * Step 3: Utilize finite state machine to determine whether a step has been detected.
     */

    switch(currentState){

    case(IDLE):
        // do what is needed in the IDLE state. We'll just stay here as an example

        /**
         * Implement the STATE actions
         */


        /**
         * Implement the STATE nextState logic
         */

        break;

    case(DETECT):
        // do what is needed in the DETECT state

        /**
        * Implement the STATE actions
        */


        /**
         * Implement the STATE nextState logic
         */
        break;

    case(TIMEOUT):
        // do what is needed in the TIMEOUT state

        /**
        * Implement the STATE actions
        */


        /**
         * Implement the STATE nextState logic
         */
        break;

    default:
        cout << "Error! This should never happen!" << endl;
        break;
    }


    // update state machine for next iteration
    currentState = nextState;

    // increment our counter to know how many iterations this has been.
    idx++;

    // return true so the main() method will continue to call us...
    return true;
}

/**
 * Abstraction layer: route to appropriate data source
 */
bool get_next_sample(AccelSample* sample)
{
    return read_csv_sample(sample);
}
