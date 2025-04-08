#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "variables.h"

// Common function prototypes
void generateSignal(uint8_t taskNumber, uint8_t outputPin, uint16_t onTime1, uint16_t onTime2);
void measureFrequency(uint8_t taskNumber, uint8_t inputPin, uint16_t minFrequency, uint16_t maxFrequency, uint16_t * frequencyVal);

#if USE_FREERTOS
// FreeRTOS task function prototypes
void vTask1Function(void *pvParameters);
void vTask2Function(void *pvParameters);
void vTask3Function(void *pvParameters);
void vTask4Function(void *pvParameters);
void vTask5Function(void *pvParameters);
void vTask6Function(void *pvParameters);
void vTask7Function(void *pvParameters);
void pushButtonISR();

#else
// Cyclic executive function prototypes
void executeTasks();
void jobTask1();
void jobTask2();
void jobTask3();
void jobTask4();
void jobTask5();
void jobTask6();
void jobTask7();
void pushButtonInterrupt();
#endif

#endif // FUNCTIONS_H