#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Wire.h>
#include <VL53L0X.h>
#include "../lib/task1.h"

VL53L0X sensor;

static void task2(void*) {
    while (true) {
        // ::Serial.println("TICK");
        // ::vTaskDelay(pdMS_TO_TICKS(500));

        // ::Serial.println("TOCK");
        // ::vTaskDelay(pdMS_TO_TICKS(500));

        ::Serial.print(sensor.readRangeContinuousMillimeters());
        if (sensor.timeoutOccurred()) {::Serial.print(" TIMEOUT"); }

        ::Serial.println();
    }
}

FLASHMEM __attribute__((noinline)) void setup() {
    ::Serial.begin(115'200);
    ::pinMode(arduino::LED_BUILTIN, arduino::OUTPUT);
    ::digitalWriteFast(arduino::LED_BUILTIN, arduino::HIGH);

    ::delay(100);

    if (CrashReport) {
        ::Serial.print(CrashReport);
        ::Serial.println();
        ::Serial.flush();
    }

    ::Serial.println(PSTR("\r\nBooting FreeRTOS kernel " tskKERNEL_VERSION_NUMBER ". Built by gcc " __VERSION__ " (newlib " _NEWLIB_VERSION ") on " __DATE__ ". ***\r\n"));

    ::Wire.begin();

    ::sensor.setTimeout(500);
    if (!::sensor.init())
    {
        while (1) {
            ::Serial.println("Failed to detect and initialize sensor!");
            ::vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
    sensor.startContinuous();

    ::xTaskCreate(task1, "task1", 128, nullptr, 2, nullptr);
    ::xTaskCreate(task2, "task2", 2048, nullptr, 2, nullptr);

    ::Serial.println("setup(): starting scheduler...");
    ::Serial.flush();

    ::vTaskStartScheduler();
}

void loop() {}
