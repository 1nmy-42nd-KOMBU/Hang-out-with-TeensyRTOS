#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <VL53L0X.h>

void task2(void*) {
    while (true) {
        ::Serial.println("TICK");
        ::vTaskDelay(pdMS_TO_TICKS(500));

        ::Serial.println("TOCK");
        ::vTaskDelay(pdMS_TO_TICKS(500));
    }
}