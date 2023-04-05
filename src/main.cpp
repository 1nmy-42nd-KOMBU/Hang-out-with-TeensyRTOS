/*
 * This file is part of the FreeRTOS port to Teensy boards.
 * Copyright (c) 2020 Timo Sandmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file    main.cpp
 * @brief   FreeRTOS example for Teensy boards
 * @author  Timo Sandmann
 * @date    17.05.2020
 */

#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

static void task1(void*) {
    while (true) {
        digitalWriteFast(arduino::LED_BUILTIN, arduino::LOW);
        ::vTaskDelay(pdMS_TO_TICKS(500));

        digitalWriteFast(arduino::LED_BUILTIN, arduino::HIGH);
        ::vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void task2(void*) {
    while (true) {
        /* Get a new sensor event */
        sensors_event_t event;
        bno.getEvent(&event);

        /* The processing sketch expects data as roll, pitch, heading */
        Serial.print("Orientation: ");
        Serial.print((float)event.orientation.x);
        Serial.print(" ");
        Serial.print((float)event.orientation.y);
        Serial.print(" ");
        Serial.print((float)event.orientation.z);
        Serial.println();

        ::vTaskDelay(pdMS_TO_TICKS(BNO055_SAMPLERATE_DELAY_MS));
    }
}

FLASHMEM __attribute__((noinline)) void setup() {
    Serial.begin(115'200);
    Wire.begin();
    /* Initialise the sensor */
    if(!bno.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
    }
    
    delay(1000);

    /* Use external crystal for better accuracy */
    bno.setExtCrystalUse(true);

    pinMode(arduino::LED_BUILTIN, arduino::OUTPUT);
    digitalWriteFast(arduino::LED_BUILTIN, arduino::HIGH);

    delay(5000);

    Serial.println(PSTR("\r\nRunning FreeRTOS kernel " tskKERNEL_VERSION_NUMBER ". Built by gcc " __VERSION__ "."));

    ::xTaskCreate(task1, "task1", 128, nullptr, 2, nullptr);
    ::xTaskCreate(task2, "task2", 128, nullptr, 2, nullptr);

    Serial.println("setup(): starting scheduler...");
    Serial.flush();

    ::vTaskStartScheduler();


}

void loop() {}
