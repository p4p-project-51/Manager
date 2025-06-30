#include "tasks/led_task.h"

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
