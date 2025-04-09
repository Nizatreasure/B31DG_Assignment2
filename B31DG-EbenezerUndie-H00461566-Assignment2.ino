#include "arduino_secrets.h"

#include "functions.h"

void setup() {
  Serial.begin(115200);
  
  // Initialize GPIO pins
  pinMode(OUTPUT_SIGNAL1_PIN, OUTPUT);
  pinMode(OUTPUT_SIGNAL2_PIN, OUTPUT);
  pinMode(INPUT_SIGNAL1_PIN, INPUT);
  pinMode(INPUT_SIGNAL2_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(PUSH_BUTTON_PIN, INPUT_PULLDOWN);
  
#if USE_FREERTOS
  // FreeRTOS implementation
  
  // Attach interrupt for push button
  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_PIN), pushButtonISR, FALLING);
  
  // Create semaphores
  frequencyMutex = xSemaphoreCreateMutex();
  buttonSemaphore = xSemaphoreCreateBinary();
  
  // Start monitor
  monitor.startMonitoring();

  Serial.println("FREE_RTOS running");
  
  // Create tasks
  xTaskCreate(
    vTask1Function,       
    "Signal1Task", 
    TASK1_STACK_SIZE,    
    NULL,                
    TASK1_PRIORITY,   
    NULL           
  );
  
  xTaskCreate(
    vTask2Function,
    "Signal2Task",
    TASK2_STACK_SIZE,
    NULL,
    TASK2_PRIORITY,
    NULL
  );
  
  xTaskCreate(
    vTask3Function,
    "Freq1Task",
    TASK3_STACK_SIZE,
    NULL,
    TASK3_PRIORITY,
    NULL
  );
  
  xTaskCreate(
    vTask4Function,
    "Freq2Task",
    TASK4_STACK_SIZE,
    NULL,
    TASK4_PRIORITY,
    NULL
  );
  
  xTaskCreate(
    vTask5Function,
    "MonitorTask",
    TASK5_STACK_SIZE,
    NULL,
    TASK5_PRIORITY,
    NULL
  );
  
  xTaskCreate(
    vTask6Function,
    "LED1Task",
    TASK6_STACK_SIZE,
    NULL,
    TASK6_PRIORITY,
    NULL
  );
  
  xTaskCreate(
    vTask7Function,
    "ButtonTask",
    TASK7_STACK_SIZE,
    NULL,
    TASK7_PRIORITY,
    NULL
  );
  
  
#else
  // Cyclic executive implementation
  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_PIN), pushButtonInterrupt, FALLING); // Attach an interrupt to the push button pin

  Serial.println("Cyclic Executive running");
  frameTicker.attach_us(FRAME_INTERVAL, executeTasks); // Set up the frame ticker
  monitor.startMonitoring();
  
#endif
}

void loop() {
}