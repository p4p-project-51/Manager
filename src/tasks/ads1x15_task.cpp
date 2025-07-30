#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "tasks/ads1x15_task.h"

Adafruit_ADS1015 ads1015_0;
Adafruit_ADS1015 ads1015_1;
Adafruit_ADS1015 ads1015_2;
Adafruit_ADS1015 ads1015_3;

// Addresses:
// ADDR -> GND 0x48 (default for unconnected ADDR pin)
// ADDR -> VDD 0x49
// ADDR -> SDA 0x4A
// ADDR -> SCL 0x4B
void pollAds1x15Task(void *pvParameters)
{
    Serial.begin(115200);
    ads1015_0.begin(0x48);
    ads1015_1.begin(0x49);
    ads1015_2.begin(0x4A);
    ads1015_3.begin(0x4B);

    while (1) {
        // Changing the data rate from the default 1600 SPS (either way) seems to negatively affect the readings

        int16_t adc0 = ads1015_0.readADC_SingleEnded(0);
        int16_t adc1 = ads1015_1.readADC_SingleEnded(0);
        int16_t adc2 = ads1015_2.readADC_SingleEnded(0);
        int16_t adc3 = ads1015_3.readADC_SingleEnded(0);

        Serial.print("ADC0_CH0: "); Serial.print(adc0); Serial.print("\t");
        Serial.print("ADC1_CH0: "); Serial.print(adc1); Serial.print("\t");
        Serial.print("ADC2_CH0: "); Serial.print(adc2); Serial.print("\t");
        Serial.print("ADC3_CH0: "); Serial.println(adc3);

        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
