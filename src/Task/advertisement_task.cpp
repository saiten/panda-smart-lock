/**
 * advertisement_task.cpp
 */
#include "task/advertisement_task.hpp"

namespace task {

static advertisement_task *current_task = NULL;

void advertisement_task::update() {
    if(current_task == NULL) {
        return;
    }

    // 一定時間おきにchallegeを更新する
    if(millis() - _last_updated_time > 15000) {
        update_challenge();
    }
}

void advertisement_task::on_attach(simple_task::task_manager *manager) {
    simple_task::task::on_attach(manager);
    current_task = this;

    main_service.begin();
    update_challenge();
    main_service.set_operation_callback(advertisement_task::on_operatation_received);

    Bluefruit.Periph.setConnectCallback(advertisement_task::on_connect);
    Bluefruit.Periph.setDisconnectCallback(advertisement_task::on_disconnect);

    Bluefruit.Advertising.clearData();
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addService(main_service);
    Bluefruit.Advertising.addName();

    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode
    Bluefruit.Advertising.start(0);             // 0 = Don't stop advertising after n seconds
}

void advertisement_task::on_detach(simple_task::task_manager *manager) {
    simple_task::task::on_detach(manager);

    Bluefruit.Advertising.stop();
    current_task = NULL;
}

void advertisement_task::on_connect(uint16_t conn_handle) {
    Serial.printf("connected.\n");
    current_task->update_challenge();
}

void advertisement_task::on_disconnect(uint16_t conn_handle, uint8_t reason) {
    Serial.printf("disconnected.\n");
}

void advertisement_task::on_operatation_received(uint8_t operation) {
    Serial.printf("operation received. operation = %d\n", operation);
}

void advertisement_task::update_challenge() {
    main_service.update_challenge();
    _last_updated_time = millis();
}

} // namespace task
