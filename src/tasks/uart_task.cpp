#include "tasks/uart_task.h"
#include <Arduino.h>
#include <queue>
#include <string>

std::queue<String> uartQueue;

void uartSend(const char *msg)
{
    uartQueue.push(String(msg));
}

void uartTask(void *pvParameters)
{
    Serial.begin(115200);

    while (true)
    {
        while (!uartQueue.empty())
        {
            Serial.println(uartQueue.front());
            uartQueue.pop();
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
