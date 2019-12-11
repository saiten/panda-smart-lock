/**
 * menu_task.hpp
 */
#pragma once

#include "constant.h"
#include <Adafruit_SSD1306.h>
#include <simple_menu.h>
#include <simple_task.h>
#include <util.h>

namespace task {

class menu_task : public simple_task::task {
  public:
    menu_task(Adafruit_SSD1306 *display)
        : task(TASK_ID_MENU), display(display), menu(simple_menu::menu(display)) {}
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

    static void select_registration_key(simple_menu::menu *menu, simple_menu::page *page, simple_menu::item *item);
    static void select_key_list(simple_menu::menu *menu, simple_menu::page *page, simple_menu::item *item);
    static void select_about(simple_menu::menu *menu, simple_menu::page *page, simple_menu::item *item);

    static void start_registration_key(simple_menu::menu *menu, simple_menu::page *page, simple_menu::item *item);

    static void select_delete_key(simple_menu::menu *menu, simple_menu::page *page, simple_menu::item *item);
    static void remove_key(simple_menu::menu *menu, simple_menu::page *page, simple_menu::item *item);
};

} // namespace task
