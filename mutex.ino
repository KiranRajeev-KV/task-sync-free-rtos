#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// Define GPIO pins for LEDs
#define LED1 18  // Change to any free GPIO pin
#define LED2 19

SemaphoreHandle_t mutex;  // Mutex handle

// Task for controlling LED1
void task1(void *parameter) {
    while (1) {
        if (xSemaphoreTake(mutex, portMAX_DELAY)) {  // Take the mutex
            digitalWrite(LED1, HIGH);   // Turn on LED1
            vTaskDelay(500 / portTICK_PERIOD_MS);  // Delay for 500ms
            digitalWrite(LED1, LOW);    // Turn off LED1
            xSemaphoreGive(mutex);      // Release the mutex
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);  // Allow other tasks to run
    }
}

// Task for controlling LED2
void task2(void *parameter) {
    while (1) {
        if (xSemaphoreTake(mutex, portMAX_DELAY)) {  // Take the mutex
            digitalWrite(LED2, HIGH);   // Turn on LED2
            vTaskDelay(500 / portTICK_PERIOD_MS);  // Delay for 500ms
            digitalWrite(LED2, LOW);    // Turn off LED2
            xSemaphoreGive(mutex);      // Release the mutex
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);  // Allow other tasks to run
    }
}

void setup() {
    // Initialize serial communication for debugging
    Serial.begin(115200);

    // Set LED pins as outputs
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    
    // Create a mutex
    mutex = xSemaphoreCreateMutex();
    
    if (mutex == NULL) {
        Serial.println("Failed to create mutex!");
        while (1);  // Stop execution if mutex creation fails
    }

    // Create two tasks for controlling the LEDs
    xTaskCreate(task1, "Task 1", 1024, NULL, 1, NULL);
    xTaskCreate(task2, "Task 2", 1024, NULL, 1, NULL);
}

void loop() {
    // Empty loop; tasks run independently
}
 
