#include "tasks/uart_task.h"
#include <Arduino.h>
#include <queue>
#include <string>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

std::queue<String> uartQueue;

#define BLE_NAME "Project #51 - Orchestrator"
BLEUUID SERVICE_UUID((uint16_t)0xFFE0);
BLEUUID CHARACTERISTIC_UUID((uint16_t)0xFFE1);

BLECharacteristic *pCharacteristic;

class MyCallbacks : public BLECharacteristicCallbacks
{
    // Received data from BLE UART
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string value = pCharacteristic->getValue();
        if (value.length() > 0)
        {
            Serial.println("*********");
            Serial.print("Received: ");
            for (int i = 0; i < value.length(); i++)
            {
                Serial.print(value[i]);
            }
            Serial.println();

            // Echo back with newline for BLE terminal compatibility
            pCharacteristic->setValue(value + "\n");
            pCharacteristic->notify();
        }
    }
};

void uartSend(const char *msg)
{
    uartQueue.push(String(msg));
}

void uartSendBluetooth(const String &msg)
{
    const size_t maxLen = 64;
    String outMsg = msg.substring(0, maxLen) + "\n";
    if (pCharacteristic)
    {
        pCharacteristic->setValue(std::string(outMsg.c_str()));
        pCharacteristic->notify();
    }
}

void uartTask(void *pvParameters)
{
    // Initialize Serial for UART communication
    Serial.begin(115200);

    // Initialize BLE device
    BLEDevice::init(BLE_NAME);

    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
    pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->addDescriptor(new BLE2902());
    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();

    Serial.println("UART + BluetoothSerial bridge started!");

    while (true)
    {
        // Print any queued messages to both UARTs
        while (!uartQueue.empty())
        {
            String msg = uartQueue.front();

            Serial.println(msg);
            uartSendBluetooth(msg);

            uartQueue.pop();
        }

        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
