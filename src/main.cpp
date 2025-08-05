#include <Arduino.h>
#include "tasks/led_task.h"
#include "tasks/uart_task.h"
#include "tasks/dac_task.h"
#include "tasks/uart_echo_task.h"
#include "tasks/ads1x15_task.h"
#include "tasks/esp32_adc_task.h"


void setup()
{
    // xTaskCreatePinnedToCore(ledTask, "LED Task", 2048, NULL, 1, &ledTaskHandle, 0);
    xTaskCreatePinnedToCore(uartTask, "UART Task", 2048, NULL, 1, NULL, 1);
    // xTaskCreatePinnedToCore(dacTask, "DAC Task", 2048, NULL, 1, &dacTaskHandle, 1);
    // xTaskCreatePinnedToCore(uartEchoTask, "UART Echo Task", 2048, NULL, 1, &uartEchoTaskHandle, 1);

    xTaskCreate(pollAds1x15Task, "ADS1015 Polling Task", 4096, NULL, 1, NULL);
    // xTaskCreate(pollEsp32AdcTask, "ESP32 ADC Polling Task", 2048, NULL, 1, NULL);
}

void loop()
{
    return;
}