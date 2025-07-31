#include "tasks/led_task.h"

#define EMERGENCY_STOP_PIN 23
#define TOGGLE_PIN 22

volatile bool emergencyStopTriggered = false;
volatile bool toggleTriggered = false;

void IRAM_ATTR emergencyStopISR()
{
    emergencyStopTriggered = true;
}

void IRAM_ATTR toggleISR()
{
    toggleTriggered = true;
}

void ledTask(void *pvParameters)
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(EMERGENCY_STOP_PIN, INPUT_PULLUP);
    pinMode(TOGGLE_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(EMERGENCY_STOP_PIN), emergencyStopISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(TOGGLE_PIN), toggleISR, CHANGE);

    bool ledState = false;
    while (true)
    {
        if (emergencyStopTriggered)
        {
            emergencyStopTriggered = false;
            // Blink LED quickly 5 times
            for (int i = 0; i < 5; ++i)
            {
                digitalWrite(LED_BUILTIN, HIGH);
                vTaskDelay(100 / portTICK_PERIOD_MS);
                digitalWrite(LED_BUILTIN, LOW);
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
        }
        if (toggleTriggered)
        {
            toggleTriggered = false;
            ledState = !ledState;
            digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // Small delay to yield
    }
}
