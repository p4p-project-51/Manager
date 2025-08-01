#include <Arduino.h>
#include "tasks/ads1x15_task.h"
#include "tasks/esp32_adc_task.h"
#include "tasks/adc_send.h"

const uint8_t esp32_adc_channel = 0; // GPIO36 (VP) is ADC1 channel 0

void pollEsp32AdcTask(void *pvParameters)
{
    Serial.begin(115200);

    while (true)
    {
        int adc_value = analogRead(esp32_adc_channel);
        AdcSample sample = {0, esp32_adc_channel, adc_value};
        sendSample(sample);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
