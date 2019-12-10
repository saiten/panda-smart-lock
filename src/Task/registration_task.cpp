/**
 * advertisement.cpp
 */
#include "task/registration_task.hpp"
#include "key_manager.hpp"
#include "task/advertisement_task.hpp"
#include "task/menu_task.hpp"
#include <util.h>

namespace task {

static registration_task *current_task = NULL;

void registration_task::update() {
    if(current_task == NULL) {
        return;
    }

    // clear display
    if(current_task->confirm_mode) {
        display->clearDisplay();
        display->setTextSize(1);
        display->setTextColor(WHITE);
        display->setCursor(0, 0);
        display->println("Register confirm");
        const char *name = registration_service.get_name();
        display->printf("NAME: %s\n", name);
        const uint8_t *p = registration_service.get_pin_code();
        display->printf("PIN: %d%d%d%d%d%d\n", p[0], p[1], p[2], p[3], p[4], p[5]);
        display->print("(A) Register (B) Abort");
        display->display();

        if(!digitalRead(BUTTON_A)) {
            key_manager manager(PUBLIC_KEY_SIZE);
            manager.save(name, registration_service.get_tmp_public_key());
            display->clearDisplay();
            display->setCursor(0, 0);
            display->println("register completed");
            display->display();
            delay(1000);
            destroy();
        } else {
            confirm_mode = false;
        }
    } else {
        display->clearDisplay();
        display->setTextSize(1);
        display->setTextColor(WHITE);
        display->setCursor(0, 0);
        display->print("Press C to return");
        display->display();

        if(!digitalRead(BUTTON_C) && millis() - _attached_time > 500) {
            destroy();
        }
    }
}

void registration_task::on_attach(simple_task::task_manager *manager) {
    simple_task::task::on_attach(manager);
    current_task = this;

    Serial.printf("start registration task.\n");

    pinMode(BUTTON_C, INPUT_PULLUP);

    registration_service.begin();
    registration_service.set_confirm_register_callback(registration_task::on_cofirm_register);

    Bluefruit.Periph.setConnectCallback(registration_task::on_connect);
    Bluefruit.Periph.setDisconnectCallback(registration_task::on_disconnect);

    Bluefruit.Advertising.clearData();
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addService(registration_service);
    Bluefruit.Advertising.addName();

    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode
    Bluefruit.Advertising.start(0);             // 0 = Don't stop advertising after n seconds

    confirm_mode = false;

    Serial.printf("registration task setup completed.\n");
}

void registration_task::on_detach(simple_task::task_manager *manager) {
    simple_task::task::on_detach(manager);
    current_task = NULL;

    Serial.printf("end registration task.\n");
    Bluefruit.Advertising.stop();

    display->clearDisplay();
    display->display();

    util::software_reset(15);
}

void registration_task::on_connect(uint16_t conn_handle) {
    Serial.printf("connected.\n");
}

void registration_task::on_disconnect(uint16_t conn_handle, uint8_t reason) {
    Serial.printf("disconnected.\n");
}

void registration_task::on_cofirm_register(ble::registration_service *service, const char *name, const uint8_t *pin_code) {
    if(current_task == NULL) {
        return;
    }
    current_task->confirm_mode = true;
}

} // namespace task
