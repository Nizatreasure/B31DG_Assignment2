#include "variables.h"

#if USE_FREERTOS
// Initialize monitor with no initial interval
B31DGCyclicExecutiveMonitor monitor(0);

// Semaphore for protecting shared data
SemaphoreHandle_t frequencyMutex = NULL;

// Semaphore for button press notification
SemaphoreHandle_t buttonSemaphore = NULL;

#else
B31DGCyclicExecutiveMonitor monitor(FRAME_INTERVAL);
Ticker frameTicker;
// Stores the current frame index
uint8_t currentFrame = 0;
// Last debounce time for push button
unsigned long lastDebounceTimeForPushButton = 0;

// Task schedule created using excel
const bool taskSchedule[NUM_OF_FRAMES][NUMBER_7] = {
{1,1,0,0,1,0,0},
{0,0,1,0,0,1,0},
{0,1,0,1,0,0,0},
{1,1,0,0,1,1,0},
{1,0,0,0,0,0,1},
{0,1,0,1,0,0,0},
{1,1,0,0,1,1,0},
{0,0,1,0,0,1,0},
{0,1,0,0,1,0,1},
{1,1,0,0,0,1,1},
{0,0,1,0,0,0,0},
{1,1,0,0,1,0,0},
{1,1,0,0,0,1,1},
{0,0,0,1,1,0,0},
{1,1,0,0,0,0,1},
{0,1,0,1,0,1,0},
{1,0,0,0,1,1,1},
{0,1,1,0,0,0,0},
{1,1,0,0,1,0,0},
{0,0,0,0,0,1,1},
{1,1,0,0,1,0,0},
{0,1,1,0,0,0,0},
{0,0,0,1,0,0,1},
{1,1,0,0,1,1,0},
{1,1,0,0,0,0,0},
{0,0,0,1,1,1,0},
{1,1,0,0,0,0,1},
{0,1,1,0,0,0,0},
{0,0,0,0,1,1,1},
{1,1,0,0,0,0,1}
};
#endif

// Frequency of signal 1
uint16_t F1 = 0;

// Frequency of signal 2
uint16_t F2 = 0;

// Push button state
bool pushButtonPressed = false;

// The debounce delay for the push button
const unsigned long debounceDelay = 20;