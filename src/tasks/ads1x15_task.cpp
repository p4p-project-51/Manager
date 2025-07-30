#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "tasks/ads1x15_task.h"

Adafruit_ADS1015 ads1015;

void pollAds1x15Task(void *pvParameters)
{
    Serial.begin(115200);
    ads1015.begin();

    while (1) {
        int16_t adc[4];
        for (int ch = 0; ch < 4; ++ch) {
            adc[ch] = ads1015.readADC_SingleEnded(ch);
        }

        Serial.print("AIN0: "); Serial.print(adc[0]); Serial.print("\t");
        Serial.print("AIN1: "); Serial.print(adc[1]); Serial.print("\t");
        Serial.print("AIN2: "); Serial.print(adc[2]); Serial.print("\t");
        Serial.print("AIN3: "); Serial.println(adc[3]);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
