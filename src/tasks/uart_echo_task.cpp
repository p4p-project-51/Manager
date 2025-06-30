#include "tasks/uart_echo_task.h"

void uartEchoTask(void *pvParameters)
{
    Serial1.begin(9600);

    while (!Serial1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    while (!Serial)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    while (true)
    {
        if (Serial1.available())
        {
            int byteRead = Serial1.read();
            Serial.write(byteRead);
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
