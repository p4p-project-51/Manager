#include <Arduino.h>
#include <math.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

#define DAC1_PIN 25  // GPIO25 (DAC1)
#define DAC2_PIN 26 // GPIO26 (DAC2)

#define SINE_LOOKUP_TABLE_SIZE 128

TaskHandle_t ledTaskHandle = NULL;
TaskHandle_t uartTaskHandle = NULL;
TaskHandle_t dacTaskHandle = NULL;

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

void dacTask(void *pvParameters)
{
    uint8_t sineLookupTable[SINE_LOOKUP_TABLE_SIZE];
    for (int i = 0; i < SINE_LOOKUP_TABLE_SIZE; ++i)
    {
        sineLookupTable[i] = (uint8_t)(127.5 * (sin(2 * M_PI * i / SINE_LOOKUP_TABLE_SIZE) + 1));
    }
    int idx = 0;
    while (1)
    {
        uint8_t val = sineLookupTable[idx];

        // Reduced resolution to avoid hardware bug on this DAC
        dacWrite(DAC1_PIN, val / 2);
        // Full precision on other DAC that is unaffected
        dacWrite(DAC2_PIN, val);

        idx = (idx + 1) % SINE_LOOKUP_TABLE_SIZE;
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    xTaskCreatePinnedToCore(ledTask, "LED Task", 2048, NULL, 1, &ledTaskHandle, 0);
    xTaskCreatePinnedToCore(uartTask, "UART Task", 2048, NULL, 1, &uartTaskHandle, 1);
    xTaskCreatePinnedToCore(dacTask, "DAC Task", 2048, NULL, 1, &dacTaskHandle, 1);
}

void loop()
{
    // vTaskDelete(NULL); // Not used, delete default loop task
    sleep(1);
}