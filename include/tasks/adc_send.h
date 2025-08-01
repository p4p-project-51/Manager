#ifndef ADC_SEND_H
#define ADC_SEND_H
#include <Arduino.h>

struct AdcSample {
    uint8_t module;
    uint8_t channel;
    int16_t value;
};

void sendSample(const AdcSample& sample);

#endif // ADC_SEND_H
