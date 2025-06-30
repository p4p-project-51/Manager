#include "tasks/dac_task.h"
#include <math.h>

#define DAC1_PIN 25 // GPIO25 (DAC1)
#define DAC2_PIN 26 // GPIO26 (DAC2)
#define SINE_LOOKUP_TABLE_SIZE 128

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

        // Reduced resolution to workaround hardware bug on this DAC
        dacWrite(DAC1_PIN, val / 2);
        dacWrite(DAC2_PIN, val);

        idx = (idx + 1) % SINE_LOOKUP_TABLE_SIZE;
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
