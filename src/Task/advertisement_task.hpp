/**
 * advertisement_task.hpp
 */
#pragma once

#include "ble/main_service.hpp"
#include "constant.h"
#include <bluefruit.h>
#include <simple_task.h>

namespace task {
class advertisement_task : public simple_task::task {
  public:
    advertisement_task() : task(TASK_ID_ADV), main_service(ble::main_service()) {}
    void update() override;
    void on_attach(simple_task::task_manager *manager) override;
    void on_detach(simple_task::task_manager *manager) override;

  private:
    ble::main_service main_service;
    uint32_t _last_updated_time;

    static void on_connect(uint16_t conn_handle);
    static void on_disconnect(uint16_t conn_handle, uint8_t reason);
    static void on_operatation_received(uint8_t operation);

    void update_challenge();
};
} // namespace task
