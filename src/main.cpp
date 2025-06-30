#include <Arduino.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

TaskHandle_t ledTaskHandle = NULL;
TaskHandle_t uartTaskHandle = NULL;

void ledTask(void *pvParameters)
{
    pinMode(LED_BUILTIN, OUTPUT);
    while (1)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        digitalWrite(LED_BUILTIN, LOW);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void uartTask(void *pvParameters)
{
    Serial.begin(115200);
    while (1)
    {
        Serial.println("Hello!");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    xTaskCreatePinnedToCore(ledTask, "LED Task", 2048, NULL, 1, &ledTaskHandle, 0);
    xTaskCreatePinnedToCore(uartTask, "UART Task", 2048, NULL, 1, &uartTaskHandle, 1);
}

void loop()
{
    // vTaskDelete(NULL); // Not used, delete default loop task
    sleep(1);
}