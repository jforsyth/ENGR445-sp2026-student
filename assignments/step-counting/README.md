# Step Counting Project

In this assignment we will examine a collection of smartphone recordings to detect when a person begins to walk and how many steps they have taken. The sensor readings are captured at ~100Hz with a three-axis accelerometer, gyroscope, and magnetometer.

## The Data

Six traces have been uploaded to the repository for evaluation. The challenge is to determine for each trace when walking begins and ends, and how many steps were taken during that period.

For the six datasets, each phone was carried by a different individual in a variety of locations on the body or in personal belongings. The official results for each, as determined by a video camera, are provided in the table below. The start/stop locations for walking are indicated by the index/position of the data in the stream and by the absolute time (samples at ~100Hz).

| Participant | Phone Location | Steps | Walking Begins (index) | Walking Ends (index) | Start (s) | End (s) |
|-------------|----------------|-------|----------------------|----------------------|-----------|---------|
| p1.1 | Handheld | 70 | 1607 | 5393 | 16.07 | 53.93 |
| p1.4 | Purse | 70 | 2110 | 5963 | 21.1 | 59.63 |
| p2.2 | Handheld | 66 | 1469 | 5930 | 14.69 | 59.3 |
| p9.2 | Pants pocket | 70 | 4774 | 8525 | 47.74 | 85.25 |
| p11.3 | Backpack | 76 | 1603 | 6709 | 16.03 | 67.09 |
| p27.1 | Handheld | 68 | 7325 | 11053 | 73.25 | 110.53 |

**Note:** Timestamps are calculated as `index / 100` (since sampling is at ~100 Hz).

---

## Project Structure

```
step-counting/
├── README.md              # This file
├── SETUP.md               # Environment setup & debugging guide
├── main.cpp               # Entry point
├── App.h                  # Declarations, AccelSample struct, CSV infrastructure
├── App.cpp                # Main algorithm, state machine skeleton
└── *.csv                  # Sample acceleration data files
```

---

## Getting Started

A sample .cpp file has been provided that will parse the data files for you. Use this as your template to implement the algorithm. Also, you will need to move the .csv files into the project directory to easily access them.

To set up your development environment, follow the [SETUP.md](SETUP.md) guide for platform-specific instructions on installing Visual Studio Code, a C++ compiler, and CMake.

## Application Architecture

This project uses the **Simplicity Studio embedded systems pattern**:

1. **`main.cpp` calls `app_init()` once at startup**
   - Initializes the CSV reader (or hardware accelerometer on embedded systems)
   - Sets up the state machine and all global data structures
   - Returns `true` on success, `false` on error

2. **`main.cpp` then calls `app_process_action()` repeatedly in a loop**
   - Called once per acceleration sample (approximately 100 times per second)
   - Returns `true` to continue looping
   - Returns `false` when done (e.g., end of data file)

3. **The loop terminates when `app_process_action()` returns `false`**

**Your implementation goes in `app_process_action()`** in `App.cpp`:
- Implement your step-detection state machine here
- Use the global arrays (`magnitude`, `diffs`, `squares`, `averages`) as needed
- Call `get_next_sample()` to acquire acceleration data
- Modify the `STATE` enum and `nextState` logic to detect walking periods

## Recommended Approaches

The paper recommends several approaches to detecting and counting steps based on their analysis:

1. A straightforward thresholding of the accelerometer standard deviation will robustly and cheaply detect periods of walk. It will, however, also produce significant false positives in general use.

2. A windowed peak detection algorithm is overall the optimal option for step counting regardless of smartphone placement.

3. When evaluating any system that uses walk detection or step counting, it is important to use the inputs from a variety of people and carrying positions, and to consider the effect of typical non-walking actions.

## Code Structure

- **`App.h`** — Declarations, `AccelSample` struct, CSV infrastructure
- **`App.cpp`** — State machine skeleton, main algorithm loop
- **`main.cpp`** — Entry point
- **`*.csv`** — Sample acceleration data files

## Global Resources

Pre-allocated arrays are available for your use:
- `float magnitude[16]` — Magnitude of acceleration
- `float diffs[16]` — Differences between samples
- `float squares[16]` — Squared values
- `float averages[16]` — Sliding window averages

## Testing

Test your implementation against all six datasets. For each, compare your detected step count and walking period against the reference table above.
