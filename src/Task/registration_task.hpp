/**
 * registration_task.hpp
 */
#pragma once

#include "ble/registration_service.hpp"
#include "constant.h"
#include <Adafruit_SSD1306.h>
#include <bluefruit.h>
#include <simple_task.h>

namespace task {
class registration_task : public simple_task::task {
  public:
    registration_task(Adafruit_SSD1306 *display)
        : task(TASK_ID_REGISTER), display(display), registration_service(ble::registration_service()) {}
    void update() override;
    void on_attach(simple_task::task_manager *manager) override;
    void on_detach(simple_task::task_manager *manager) override;

  private:
    Adafruit_SSD1306 *display;
    ble::registration_service registration_service;
    bool confirm_mode = false;
    uint32_t _last_updated_time;

    static void on_connect(uint16_t conn_handle);
    static void on_disconnect(uint16_t conn_handle, uint8_t reason);
    static void on_cofirm_register(ble::registration_service *service, const char *name, const uint8_t *pin_code);
};
} // namespace task
