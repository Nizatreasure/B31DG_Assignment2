# B31DG: Embedded Software Assignment 2

This project implements two versions of a machine monitor system on the ESP32 platform:
1. A Cyclic Executive implementation
2. A FreeRTOS-based implementation

Both implementations satisfy the same seven functional requirements and meet specified real-time deadlines.

## Project Structure

```
B31DG_Assignment2/
├── B31DG_Assignment2.ino      # Main Arduino sketch file
├── config.h                   # Configuration file to switch between implementations
├── definitions.h              # Project-wide definitions and constants
├── functions.cpp              # Implementation of all task functions
├── functions.h                # Function declarations
├── README.md                  # This file
├── sketch.json                # Arduino IDE project file
├── variables.cpp              # Global variable definitions
└── variables.h                # Global variable declarations
```

## Requirements

The system implements the following requirements:

1. Output a digital signal: HIGH for 250μs, LOW for 50μs, HIGH for 300μs, then LOW
2. Output a second digital signal: HIGH for 100μs, LOW for 50μs, HIGH for 200μs, then LOW
3. Measure the frequency of a square wave signal (666Hz-1000Hz)
4. Measure the frequency of a second square wave signal (833Hz-1500Hz)
5. Call the monitor's method `doWork()` at a rate of 200Hz
6. Use an LED to indicate when the sum of the two frequencies is greater than 1500Hz
7. Monitor a pushbutton to toggle an LED state and call `doWork()` when pressed

## Hardware Setup

- ESP32 development board
- External function generator (to provide input signals for requirements 3 and 4)
- Pushbutton connected to pin 32
- LEDs connected to pins 18 and 19
- Digital output signals on pins 26 and 27
- Input signal connections on pins 16 and 17

## Usage

### Switching Between Implementations

To switch between the Cyclic Executive and FreeRTOS implementations, modify the `config.h` file:

```cpp
// Set to 1 to use FreeRTOS implementation, 0 for cyclic executive
#define USE_FREERTOS 0
```

Change the value to `1` for FreeRTOS or `0` for Cyclic Executive implementation.

### Building and Uploading

1. Open the project in Arduino IDE or PlatformIO
2. Select your ESP32 board
3. Build and upload the project
4. Monitor the system behavior using serial monitor and oscilloscope

## Cyclic Executive Implementation

The Cyclic Executive implementation uses a pre-computed task schedule table with 30 frames. 
A Ticker interrupt triggers each frame every 2ms, executing the required tasks based on the schedule.

## FreeRTOS Implementation

The FreeRTOS implementation creates dedicated tasks for each requirement, using:
- Periodic tasks with appropriate priorities and stack sizes
- Semaphores for synchronization
- Mutexes to prevent race conditions when accessing shared data

## Monitoring
The project uses the provided B31DGCyclicExecutiveMonitor library to verify that all tasks meet their real-time requirements.
