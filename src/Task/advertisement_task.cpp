/**
 * advertisement.cpp
 */
#include "task/advertisement_task.hpp"

namespace task {

void advertisement_task::update() {
    // do nothing
}

void advertisement_task::on_attach(simple_task::task_manager *manager) {
    Bluefruit.begin();
    Bluefruit.setName("Panda Smart Key");
    Bluefruit.Periph.setConnectCallback(advertisement_task::on_connect);
    Bluefruit.Periph.setDisconnectCallback(advertisement_task::on_disconnect);

    deviceService.setManufacturer("iside system");
    deviceService.setModel("Panda Smart Key");
    deviceService.begin();

    mainService.begin();

    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();

    Bluefruit.Advertising.addService(mainService);
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode
    Bluefruit.Advertising.start(0);             // 0 = Don't stop advertising after n seconds
}

void advertisement_task::on_detach(simple_task::task_manager *manager) {
    Bluefruit.Advertising.stop();
}

void advertisement_task::on_connect(uint16_t conn_handle) {
}

void advertisement_task::on_disconnect(uint16_t conn_handle, uint8_t reason) {
}
} // namespace task
