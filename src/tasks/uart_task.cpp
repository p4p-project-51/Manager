#include "tasks/uart_task.h"

void uartTask(void *pvParameters)
{
    Serial.begin(115200);

    while (1)
    {
        Serial.println("Hello!");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
