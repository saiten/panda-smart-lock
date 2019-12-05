/**
 * menu_task.hpp
 */
#pragma once

#include <Adafruit_SSD1306.h>
#include <simple_menu.h>
#include <simple_task.h>
#include <util.h>

namespace task {

class menu_task : public simple_task::task {
  public:
    menu_task(Adafruit_SSD1306 *display)
        : display(display), menu(simple_menu::menu(display)) {}
    void update() override;
    void on_attach(simple_task::task_manager *manager) override;
    void on_detach(simple_task::task_manager *manager) override;

  private:
    const uint32_t suspend_time = 10000;
    Adafruit_SSD1306 *display;
    uint32_t last_operation_time;
    simple_menu::menu menu;
    bool display_sleeped;
    int last_input_button;

    int get_input_button();
    void sleep_display();
    void wake_display();
};

} // namespace task
