/**
 * advertisement.hpp
 */
#pragma once

#include <bluefruit.h>
#include <simple_task.h>

#define UUID16_SVC_MAIN 0x08FD

namespace task {
class advertisement_task : public simple_task::task {
  public:
    advertisement_task() {}
    void update() override;
    void on_attach(simple_task::task_manager *manager) override;
    void on_detach(simple_task::task_manager *manager) override;

  private:
    BLEService mainService = BLEService(UUID16_SVC_MAIN);
    BLEDis deviceService;
    static void on_connect(uint16_t conn_handle);
    static void on_disconnect(uint16_t conn_handle, uint8_t reason);
};
} // namespace task

// namespace ble {
//   class MainService: public BLEService {
//     MainService() : BLEService(UUID16_SVC_MAIN) {

//     }
//   }
// }