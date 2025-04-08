#include "functions.h"

// This function generates signal for task1 and task2
void generateSignal(uint8_t taskNumber, uint8_t outputPin, uint16_t onTime1, uint16_t onTime2)
{ 
  monitor.jobStarted(taskNumber);
  digitalWrite(outputPin, HIGH);
  delayMicroseconds(onTime1);
  digitalWrite(outputPin, LOW);
  delayMicroseconds(DELAY_50);
  digitalWrite(outputPin, HIGH);
  delayMicroseconds(onTime2);
  digitalWrite(outputPin, LOW);
  monitor.jobEnded(taskNumber);
}

// This function measures and calculates the frequency of an input wave
void measureFrequency(uint8_t taskNumber, uint8_t inputPin, uint16_t minFrequency, uint16_t maxFrequency, uint16_t * frequencyVal) 
{ 

  monitor.jobStarted(taskNumber);
  unsigned long startTime, period;
  // Get the state of the signal
  bool pinState = digitalRead(inputPin);

  // Used to exit the function if no signal is found
  unsigned long startWaitTime = micros();
  // Wait for the signal state to change
  while (digitalRead(inputPin) == pinState) {
    // If no input waveform is detected, exit the function
     if (micros() - startWaitTime > maxFrequency) {
      *frequencyVal = 0;
      monitor.jobEnded(taskNumber);
      return;
    }
  }
  // Record the time
  startTime = micros();
  pinState = !pinState;
  // Wait for the signal state to change again
  while (digitalRead(inputPin) == pinState);
  // Calculate the period and frequency
  period = micros() - startTime;
  // Exit the function if the period is less than 1 as the frequency would be invalid if calculated
  if (period < NUMBER_1) 
  {
    *frequencyVal = 0;
    monitor.jobEnded(taskNumber);
    return;
  }
  // Calculate the frequency of the square wave
  uint16_t frequency = MICROSECONDS_PER_SECOND / (period  * NUMBER_2 );
  // Exit the function if the frequency doesn't fall within the specified range of frequencies
  if (frequency < minFrequency || frequency > maxFrequency) 
  { 
    // Reset the frequency if it does not meet the specified requirement
    *frequencyVal = 0;
    monitor.jobEnded(taskNumber);
    return;
  }
  // Set the frequency value
  *frequencyVal = frequency;
  monitor.jobEnded(taskNumber);
  // Serial.println("Frequency is: "); 
  // Serial.println(frequency);
}

#if USE_FREERTOS

// ISR for push button press
void pushButtonISR()
{
  static unsigned long lastDebounceTime = 0;
  unsigned long currentTime = millis();
  
  if (currentTime - lastDebounceTime > debounceDelay) {
    lastDebounceTime = currentTime;
    
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(buttonSemaphore, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
      portYIELD_FROM_ISR();
    }
  }
}

// Task 1: Generate the first digital signal
void vTask1Function(void *pvParameters)
{
  TickType_t xLastWakeTime;
  const TickType_t xPeriod = pdMS_TO_TICKS(TASK1_PERIOD);
  
  // Initialize the xLastWakeTime variable with the current time
  xLastWakeTime = xTaskGetTickCount();
  
  for (;;) {
    generateSignal(NUMBER_1, OUTPUT_SIGNAL1_PIN, DELAY_250, DELAY_300);
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }
}

// Task 2: Generate the second digital signal
void vTask2Function(void *pvParameters)
{
  TickType_t xLastWakeTime;
  const TickType_t xPeriod = pdMS_TO_TICKS(TASK2_PERIOD);
  
  // Initialize the xLastWakeTime variable with the current time
  xLastWakeTime = xTaskGetTickCount();
  
  for (;;) {
    generateSignal(NUMBER_2, OUTPUT_SIGNAL2_PIN, DELAY_100, DELAY_200);
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }
}

// Task 3: Measure the frequency of first signal
void vTask3Function(void *pvParameters)
{
  TickType_t xLastWakeTime;
  const TickType_t xPeriod = pdMS_TO_TICKS(TASK3_PERIOD);
  
  // Initialize the xLastWakeTime variable with the current time
  xLastWakeTime = xTaskGetTickCount();
  uint16_t frequency;
  
  for (;;) {
    measureFrequency(NUMBER_3, INPUT_SIGNAL1_PIN, MIN_FREQUENCY_SIGNAL_1, MAX_FREQUENCY_SIGNAL_1, &frequency);
    
    // Update shared variable with mutex protection
    if (xSemaphoreTake(frequencyMutex, pdMS_TO_TICKS(1)) == pdTRUE) {
      F1 = frequency;
      xSemaphoreGive(frequencyMutex);
    }
    
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }
}

// Task 4: Measure the frequency of second signal
void vTask4Function(void *pvParameters)
{
  TickType_t xLastWakeTime;
  const TickType_t xPeriod = pdMS_TO_TICKS(TASK4_PERIOD);
  
  // Initialize the xLastWakeTime variable with the current time
  xLastWakeTime = xTaskGetTickCount();
  uint16_t frequency;
  
  for (;;) {
    measureFrequency(NUMBER_4, INPUT_SIGNAL2_PIN, MIN_FREQUENCY_SIGNAL_2, MAX_FREQUENCY_SIGNAL_2, &frequency);
    
    // Update shared variable with mutex protection
    if (xSemaphoreTake(frequencyMutex, pdMS_TO_TICKS(1)) == pdTRUE) {
      F2 = frequency;
      xSemaphoreGive(frequencyMutex);
    }
    
    // Wait for the next cycle
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }
}

// Task 5: Call the monitor's doWork method
void vTask5Function(void *pvParameters)
{
  TickType_t xLastWakeTime;
  const TickType_t xPeriod = pdMS_TO_TICKS(TASK5_PERIOD);
  
  // Initialize the xLastWakeTime variable with the current time
  xLastWakeTime = xTaskGetTickCount();
  
  for (;;) {
    monitor.jobStarted(NUMBER_5);
    monitor.doWork();
    monitor.jobEnded(NUMBER_5);
    
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }
}

// Task 6: Control LED1 based on the sum of frequencies
void vTask6Function(void *pvParameters)
{
  uint16_t f1Value, f2Value;
  
  for (;;) {
    // Get current frequency values with mutex protection
    if (xSemaphoreTake(frequencyMutex, pdMS_TO_TICKS(1)) == pdTRUE) {
      f1Value = F1;
      f2Value = F2;
      xSemaphoreGive(frequencyMutex);
    }
    
    // Set LED based
    if (f1Value + f2Value > NUMBER_1500) {
      digitalWrite(LED1_PIN, HIGH);
    } else {
      digitalWrite(LED1_PIN, LOW);
    }
    
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

// Task 7: Toggle LED2 on push button press
void vTask7Function(void *pvParameters)
{
  for (;;) {
    // Wait for semaphore indicating button press
    if (xSemaphoreTake(buttonSemaphore, portMAX_DELAY) == pdTRUE) {

      bool ledState = digitalRead(LED2_PIN);
      digitalWrite(LED2_PIN, !ledState);
      
      monitor.doWork();
    }
  }
}

#else

// Execute tasks based on the current frame
void executeTasks() {
    if (taskSchedule[currentFrame][NUMBER_0]) jobTask1();
    if (taskSchedule[currentFrame][NUMBER_1]) jobTask2();
    if (taskSchedule[currentFrame][NUMBER_2]) jobTask3();
    if (taskSchedule[currentFrame][NUMBER_3]) jobTask4();
    if (taskSchedule[currentFrame][NUMBER_4]) jobTask5();
    if (taskSchedule[currentFrame][NUMBER_5]) jobTask6();
    if (taskSchedule[currentFrame][NUMBER_6]) jobTask7();

    // Move to the next frame
    currentFrame = (currentFrame + NUMBER_1) % NUM_OF_FRAMES;
}

void jobTask1()
{
  generateSignal(NUMBER_1, OUTPUT_SIGNAL1_PIN, DELAY_250, DELAY_300);
}

void jobTask2()
{
  generateSignal(NUMBER_2, OUTPUT_SIGNAL2_PIN, DELAY_100, DELAY_200);
}

void jobTask3()
{
  measureFrequency(NUMBER_3, INPUT_SIGNAL1_PIN, MIN_FREQUENCY_SIGNAL_1, MAX_FREQUENCY_SIGNAL_1, &F1);
}

void jobTask4()
{
  measureFrequency(NUMBER_4, INPUT_SIGNAL2_PIN, MIN_FREQUENCY_SIGNAL_2, MAX_FREQUENCY_SIGNAL_2, &F2);
}

// Takes 0.6ms
void jobTask5()
{ 
  monitor.jobStarted(NUMBER_5);
  monitor.doWork();
  monitor.jobEnded(NUMBER_5);
}

/* This function turns on an LED if the sum of the two detected frequencies is above 1500
   and turns the LED off if it is 1500 or lower */
void jobTask6()
{ 
  if (F1+F2 > NUMBER_1500)
  {
    digitalWrite(LED1_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED1_PIN, LOW);
  }
}

// This function toggles the LED if the push button is pressed
void jobTask7()
{ 
  if (pushButtonPressed) {
    pushButtonPressed = false;
    bool ledState = digitalRead(LED2_PIN);
    digitalWrite(LED2_PIN, !ledState);
    monitor.doWork();
  }
}

// Interrupt function to handle push button press
void pushButtonInterrupt()
{
  unsigned long currentTime = millis(); 
  if (currentTime - lastDebounceTimeForPushButton > debounceDelay) // Debounce interval to prevent false readings
  {
    pushButtonPressed = true;
    lastDebounceTimeForPushButton = currentTime;
  }
}
#endif