#include <Arduino.h>
#include "tasks/led_task.h"
#include "tasks/uart_task.h"
#include "tasks/dac_task.h"
#include "tasks/uart_echo_task.h"

TaskHandle_t ledTaskHandle = NULL;
TaskHandle_t uartTaskHandle = NULL;
TaskHandle_t dacTaskHandle = NULL;
TaskHandle_t uartEchoTaskHandle = NULL;

void setup()
{
    xTaskCreatePinnedToCore(ledTask, "LED Task", 2048, NULL, 1, &ledTaskHandle, 0);
    xTaskCreatePinnedToCore(uartTask, "UART Task", 2048, NULL, 1, &uartTaskHandle, 1);
    xTaskCreatePinnedToCore(dacTask, "DAC Task", 2048, NULL, 1, &dacTaskHandle, 1);
    xTaskCreatePinnedToCore(uartEchoTask, "UART Echo Task", 2048, NULL, 1, &uartEchoTaskHandle, 1);
}

void loop()
{
    return;
}