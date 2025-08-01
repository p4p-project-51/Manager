#pragma once
#include <Arduino.h>

void uartTask(void *pvParameters);
void uartSend(const char* msg);
