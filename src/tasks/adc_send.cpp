#include "tasks/adc_send.h"
#include "tasks/uart_task.h"

void sendSample(const AdcSample &sample)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "M%d:C%d:V%d", sample.module, sample.channel, sample.value);
    uartSend(buf);
}
