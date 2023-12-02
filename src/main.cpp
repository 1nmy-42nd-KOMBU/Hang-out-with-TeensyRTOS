#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Wire.h>
#include "../lib/task1.h"
#include "../lib/task2.h"

TaskHandle_t task1h, task2h;

FLASHMEM __attribute__((noinline)) void setup() {
    ::Serial.begin(115'200);
    ::pinMode(arduino::LED_BUILTIN, arduino::OUTPUT);
    ::digitalWriteFast(arduino::LED_BUILTIN, arduino::HIGH);

    ::delay(10000);

    if (CrashReport) {
        ::Serial.print(CrashReport);
        ::Serial.println();
        ::Serial.flush();
    }

    ::Serial.println(PSTR("\r\nBooting FreeRTOS kernel " tskKERNEL_VERSION_NUMBER ". Built by gcc " __VERSION__ " (newlib " _NEWLIB_VERSION ") on " __DATE__ ". ***\r\n"));

    ::xTaskCreate(task1, "task1", 128, nullptr, 2, &task1h);
    ::xTaskCreate(task2, "task2", 2048, nullptr, 2, &task2h);

    ::Serial.println("setup(): starting scheduler...");
    ::Serial.flush();

    ::vTaskSuspend(task2h);
    ::vTaskStartScheduler();
}

void loop() {}
