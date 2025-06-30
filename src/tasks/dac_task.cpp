#include "tasks/dac_task.h"

#define DAC1_PIN 25 // GPIO25 (DAC1)
#define DAC2_PIN 26 // GPIO26 (DAC2)
#define ADC_INPUT_PIN 34 // GPIO34 (ADC1_CH6, input only)

void dacTask(void *pvParameters)
{
    // Configure ADC input pin
    pinMode(ADC_INPUT_PIN, INPUT);

    while (1)
    {
        int adcValue = analogRead(ADC_INPUT_PIN);
        // Scale 12-bit to 8-bit (0-255)
        uint8_t dacValue = adcValue >> 4;

        // Reduced resolution to workaround hardware bug on this DAC
        dacWrite(DAC1_PIN, dacValue / 2 + 127);
        dacWrite(DAC2_PIN, dacValue);

        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
