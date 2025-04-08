#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdint.h>
#include "B31DGMonitor.h"
#include "definitions.h"
#include "config.h"

#if USE_FREERTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#else

#include <Ticker.h>
#endif

extern B31DGCyclicExecutiveMonitor monitor;

#if USE_FREERTOS

// Semaphore for protecting shared data
extern SemaphoreHandle_t frequencyMutex;

// Semaphore for button press notification
extern SemaphoreHandle_t buttonSemaphore;

#else

extern Ticker frameTicker;
// Stores the current frame index
extern uint8_t currentFrame;
// Last debounce time for push button
extern unsigned long lastDebounceTimeForPushButton;
// Task schedule created using excel
extern const bool taskSchedule[NUM_OF_FRAMES][NUMBER_7];
#endif

// Frequency of signal 1
extern uint16_t F1;

// Frequency of signal 2
extern uint16_t F2;

// Push button state
extern bool pushButtonPressed;

// The debounce delay for the push button
extern const unsigned long debounceDelay;

#endif