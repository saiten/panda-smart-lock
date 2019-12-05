/**
 * main.ino
 */
#include "task/advertisement_task.hpp"
#include "task/splash_task.hpp"
#include <Adafruit_SSD1306.h>
#include <simple_task.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
simple_task::task_manager task_manager = simple_task::task_manager();

void setup() {
    Serial.begin(115200);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

    // start task
    auto *task = new task::splash_task(&display);
    task_manager.add(task);
    auto *adv_task = new task::advertisement_task();
    task_manager.add(adv_task);
}

void loop() {
    task_manager.update();
    yield();
}
