/**
 * splash_task.hpp
 */
#pragma once

#include <Adafruit_SSD1306.h>
#include <simple_task.h>

namespace task {
class splash_task : public simple_task::task {
  public:
    splash_task(Adafruit_SSD1306 *display) : display(display) {}
    void update() override;
    void on_attach(simple_task::task_manager *manager) override;
    void on_detach(simple_task::task_manager *manager) override;

  private:
    Adafruit_SSD1306 *display;
};

} // namespace task
