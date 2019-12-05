/**
 * menu_task.cpp
 */
#include "task/menu_task.hpp"
#include "constant.h"

namespace task {
void menu_task::on_attach(simple_task::task_manager *manager) {
    Serial.println("attached menu task");

    simple_task::task::on_attach(manager);

    // clear display
    display->clearDisplay();
    display->display();

    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(BUTTON_C, INPUT_PULLUP);

    if(menu.size() == 0) {
        auto *register_page = new simple_menu::page("Register Key");
        register_page->append(new simple_menu::item("1.Start Advertising", NULL, NULL, NULL));
        register_page->append(new simple_menu::back_item());

        auto *main_page = new simple_menu::page("Panda SmartLock");
        main_page->append(new simple_menu::item("1.Register Key", register_page, NULL, NULL));
        main_page->append(new simple_menu::item("2.Key List", NULL, NULL, NULL));
        main_page->append(new simple_menu::item("3.About", NULL, NULL, NULL));

        menu.push(main_page);
    }

    last_operation_time = millis();
    last_input_button = 0;
    display_sleeped = false;
}

void menu_task::on_detach(simple_task::task_manager *manager) {
    simple_task::task::on_detach(manager);
    Serial.println("detached menu task");
}

void menu_task::update() {
    int input_button = get_input_button();
    if(display_sleeped) {
        if(input_button != 0) {
            Serial.println("wake display");
            wake_display();
            menu.display();
            display_sleeped = false;
            last_operation_time = millis();
            last_input_button = input_button;
        }
    } else {
        if(input_button != last_input_button) {
            if(input_button == BUTTON_A) {
                menu.up();
            } else if(input_button == BUTTON_B) {
                menu.down();
            } else if(input_button == BUTTON_C) {
                menu.enter();
            }
            if(input_button != 0) {
                last_operation_time = millis();
            }
        }
        last_input_button = input_button;

        menu.display();

        if(last_operation_time + suspend_time < millis()) {
            Serial.println("sleep display");
            sleep_display();
            display_sleeped = true;
        }
    }
}

void menu_task::sleep_display() { display->ssd1306_command(SSD1306_DISPLAYOFF); }
void menu_task::wake_display() { display->ssd1306_command(SSD1306_DISPLAYON); }

int menu_task::get_input_button() {
    if(!digitalRead(BUTTON_A)) {
        return BUTTON_A;
    } else if(!digitalRead(BUTTON_B)) {
        return BUTTON_B;
    } else if(!digitalRead(BUTTON_C)) {
        return BUTTON_C;
    }
    return 0;
}

} // namespace task