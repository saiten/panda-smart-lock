/**
 * main.ino
 */
#include "task/registration_task.hpp"
#include "task/splash_task.hpp"
#include <Adafruit_SSD1306.h>
#include <InternalFileSystem.h>
#include <bluefruit.h>
#include <simple_task.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
simple_task::task_manager task_manager = simple_task::task_manager();
BLEDis device_service;

void setup() {
    Serial.begin(115200);
    Serial.println("Start panda smart key");

    randomSeed(analogRead(0));

    setup_pin();
    setup_file_system();
    setup_ble();

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

    // start task
    if(InternalFS.exists(REGISTER_MODE_FILE_PATH)) {
        InternalFS.remove(REGISTER_MODE_FILE_PATH);
        auto *task = new task::registration_task(&display);
        task_manager.add(task);
    } else {
        auto *task = new task::splash_task(&display);
        task_manager.add(task);
    }
}

void loop() {
    task_manager.update();
    yield();
}

void setup_pin() {
    digitalWrite(PIN_LOCK, HIGH);
    digitalWrite(PIN_UNLOCK, HIGH);
    digitalWrite(PIN_WINKER, HIGH);
    pinMode(PIN_LOCK, OUTPUT);
    pinMode(PIN_UNLOCK, OUTPUT);
    pinMode(PIN_WINKER, OUTPUT);
}

void setup_file_system() {
    InternalFS.begin();
    if(!InternalFS.exists(BASE_DIR)) {
        InternalFS.mkdir(BASE_DIR);
    }
    if(!InternalFS.exists(PUB_KEY_DIR)) {
        InternalFS.mkdir(PUB_KEY_DIR);
    }
}

void setup_ble() {
    Bluefruit.begin();
    Bluefruit.autoConnLed(false);
    Bluefruit.setTxPower(0);
    Bluefruit.setName("Panda Smart Key");

    device_service.setManufacturer("iside system");
    device_service.setModel("Panda Smart Key");
    device_service.begin();
}