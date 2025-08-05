#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "tasks/ads1x15_task.h"
#include "tasks/adc_send.h"

Adafruit_ADS1015 ads1015_modules[4];
const uint8_t ads_addresses[4] = {0x48, 0x49, 0x4A, 0x4B};
const uint8_t num_modules = 4;
const uint8_t num_channels = 4;

// Addresses:
// ADDR -> GND 0x48 (default for unconnected ADDR pin)
// ADDR -> VDD 0x49
// ADDR -> SDA 0x4A
// ADDR -> SCL 0x4B
void pollAds1x15Task(void *pvParameters)
{
    Serial.begin(115200);

    for (uint8_t i = 0; i < num_modules; ++i)
    {
        ads1015_modules[i].begin(ads_addresses[i]);
        ads1015_modules[i].setGain(GAIN_TWO); // Set gain to 2x
    }

    while (true)
    {
        // Changing the data rate from the default 1600 SPS (either way) seems to negatively affect the readings
        for (uint8_t ch = 0; ch < num_channels; ++ch)
        {
            for (uint8_t mod = 0; mod < num_modules; ++mod)
            {
                int16_t adc = ads1015_modules[mod].readADC_SingleEnded(ch);
                AdcSample sample = {mod, ch, adc};
                sendSample(sample);
            }
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
