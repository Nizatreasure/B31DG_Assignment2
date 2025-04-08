#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "config.h"

// Pin definitions
#define OUTPUT_SIGNAL1_PIN 26
#define OUTPUT_SIGNAL2_PIN 27
#define INPUT_SIGNAL1_PIN 16
#define INPUT_SIGNAL2_PIN 17
#define LED1_PIN 18
#define LED2_PIN 19
#define PUSH_BUTTON_PIN 32

// Timing constants
#define MICROSECONDS_PER_SECOND 1000000UL
#define DELAY_50 50
#define DELAY_100 100
#define DELAY_150 150
#define DELAY_200 200
#define DELAY_250 250
#define DELAY_300 300

// Utility numeric constants
#define NUMBER_0 0
#define NUMBER_1 1
#define NUMBER_2 2
#define NUMBER_3 3
#define NUMBER_4 4
#define NUMBER_5 5
#define NUMBER_6 6
#define NUMBER_7 7
#define NUMBER_1500 1500

// Frequency limits
#define MIN_FREQUENCY_SIGNAL_1 666
#define MAX_FREQUENCY_SIGNAL_1 1000
#define MIN_FREQUENCY_SIGNAL_2 833
#define MAX_FREQUENCY_SIGNAL_2 1500

// Cyclic executive specific constants
#define NUM_OF_FRAMES 30
#define FRAME_INTERVAL 2000

#if USE_FREERTOS
// Task periods in milliseconds
#define TASK1_PERIOD 4   
#define TASK2_PERIOD 3    
#define TASK3_PERIOD 10 
#define TASK4_PERIOD 10  
#define TASK5_PERIOD 5

// Task priorities
#define TASK1_PRIORITY 4
#define TASK2_PRIORITY 5
#define TASK3_PRIORITY 2
#define TASK4_PRIORITY 2
#define TASK5_PRIORITY 3
#define TASK6_PRIORITY 1
#define TASK7_PRIORITY 6

// Task stack sizes
#define TASK1_STACK_SIZE 2048
#define TASK2_STACK_SIZE 2048
#define TASK3_STACK_SIZE 2048
#define TASK4_STACK_SIZE 2048
#define TASK5_STACK_SIZE 1024
#define TASK6_STACK_SIZE 1024
#define TASK7_STACK_SIZE 1024
#endif

#endif