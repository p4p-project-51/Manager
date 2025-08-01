#include <Arduino.h>
#include "tasks/ads1x15_task.h"
#include "tasks/esp32_adc_task.h"

const uint8_t esp32_adc_channel = 0; // GPIO36 (VP) is ADC1 channel 0

void pollEsp32AdcTask(void *pvParameters)
{
    Serial.begin(115200);
    while (true)
    {
        int adc_value = analogRead(esp32_adc_channel);
        Serial.print("ESP32_ADC_CH");
        Serial.print(esp32_adc_channel);
        Serial.print(": ");
        Serial.println(adc_value);
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
