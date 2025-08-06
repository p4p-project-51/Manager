#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "tasks/ads1x15_task.h"
#include "tasks/adc_send.h"
#include "tasks/uart_task.h"
#include <vector>
#include <ArduinoJson.h>

Adafruit_ADS1015 ads1015_modules[4];
const uint8_t ads_addresses[4] = {0x48, 0x49, 0x4A, 0x4B};
const uint8_t num_modules = 4;
const uint8_t num_channels = 4;

// Addresses:
// ADDR -> GND 0x48 (default for unconnected ADDR pin)
// ADDR -> VDD 0x49
// ADDR -> SDA 0x4A
// ADDR -> SCL 0x4B
String batchFirmwareId;
void generateBatchFirmwareId()
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char buf[10];
    for (int i = 0; i < 9; ++i)
    {
        buf[i] = charset[random(0, sizeof(charset) - 2)];
    }
    buf[9] = '\0';
    batchFirmwareId = String(buf);
}

void sendBatchSamples(const std::vector<AdcSample> &batch)
{
    JsonDocument doc;
    doc["id"] = batchFirmwareId;
    doc["timestamp"] = (int)millis();
    JsonArray header = doc["header"].to<JsonArray>();
    JsonArray values = doc["values"].to<JsonArray>();
    for (const auto &sample : batch)
    {
        char sensorName[16];
        snprintf(sensorName, sizeof(sensorName), "M%d-C%d", sample.module, sample.channel);
        header.add(sensorName);
        values.add((int)sample.value);
    }
    String out;
    serializeJson(doc, out);
    uartSend(out.c_str());
}

void pollAds1x15Task(void *pvParameters)
{
    Serial.begin(115200);
    randomSeed(analogRead(0) + millis());
    generateBatchFirmwareId();

    for (uint8_t i = 0; i < num_modules; ++i)
    {
        ads1015_modules[i].begin(ads_addresses[i]);
        ads1015_modules[i].setGain(GAIN_TWO); // Set gain to 2x
    }

    while (true)
    {
        std::vector<AdcSample> batch;
        for (uint8_t ch = 0; ch < num_channels; ++ch)
        {
            // Start conversion for all modules
            for (uint8_t mod = 0; mod < num_modules; ++mod)
            {
                ads1015_modules[mod].startADCReading(MUX_BY_CHANNEL[ch], /*continuous=*/false);
            }

            // From https://github.com/sparkfun/SparkFun_ADS1015_Arduino_Library/blob/d777dd3083447b3da41d429cffdc90d3cd1109ca/src/SparkFun_ADS1015_Arduino_Library.cpp#L487
            vTaskDelay(1 / portTICK_PERIOD_MS);

            // Poll for completion and read results
            for (uint8_t mod = 0; mod < num_modules; ++mod)
            {
                // Wait for conversion to complete, but yield to avoid busy-waiting
                while (!ads1015_modules[mod].conversionComplete())
                {
                    vTaskDelay(1 / portTICK_PERIOD_MS);
                }

                int16_t adc = ads1015_modules[mod].getLastConversionResults();
                batch.push_back({mod, ch, adc});
            }
        }
        sendBatchSamples(batch);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
