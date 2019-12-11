/**
 * menu_task.cpp
 */
#include "task/menu_task.hpp"
#include "constant.h"
#include "key_manager.hpp"
#include "task/registration_task.hpp"
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include <util.h>

namespace task {
static menu_task *current_task = NULL;

void menu_task::on_attach(simple_task::task_manager *manager) {
    simple_task::task::on_attach(manager);
    current_task = this;

    // clear display
    display->clearDisplay();
    display->display();

    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(BUTTON_C, INPUT_PULLUP);

    if(menu.size() == 0) {
        // メインメニュー
        auto *main_page = new simple_menu::page("Panda SmartLock");
        main_page->append(new simple_menu::item("1.Register Key", NULL, NULL, menu_task::select_registration_key));
        main_page->append(new simple_menu::item("2.Key List", NULL, NULL, menu_task::select_key_list));
        main_page->append(new simple_menu::item("3.About", NULL, NULL, menu_task::select_about));
        menu.push(main_page);
    }

    last_operation_time = millis();
    last_input_button = 0;
    display_sleeped = false;
}

void menu_task::on_detach(simple_task::task_manager *manager) {
    simple_task::task::on_detach(manager);
    current_task = NULL;
}

void menu_task::update() {
    if(current_task == NULL) {
        return;
    }

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
    // 起動直後は別タスクのボタン処理を引きずっているので少し無視する
    if(millis() - _attached_time <= 500) {
        return 0;
    }
    if(!digitalRead(BUTTON_A)) {
        return BUTTON_A;
    } else if(!digitalRead(BUTTON_B)) {
        return BUTTON_B;
    } else if(!digitalRead(BUTTON_C)) {
        return BUTTON_C;
    }
    return 0;
}

void menu_task::select_registration_key(simple_menu::menu *menu, simple_menu::page *page, simple_menu::item *item) {
    // 登録
    auto *subpage = new simple_menu::page("Register Key");
    subpage->append(new simple_menu::item("1.Start", NULL, NULL, menu_task::start_registration_key));
    subpage->append(new simple_menu::back_item());
    menu->push(subpage);
}

void menu_task::start_registration_key(simple_menu::menu *menu, simple_menu::page *page, simple_menu::item *item) {
    if(current_task == NULL) {
        return;
    }
    Serial.println("select registration key");

    current_task->display->clearDisplay();
    current_task->display->display();

    // 登録モード用ファイル作成
    Adafruit_LittleFS_Namespace::File file(REGISTER_MODE_FILE_PATH, Adafruit_LittleFS_Namespace::FILE_O_WRITE, InternalFS);
    VERIFY(file, )
    file.write("_");
    file.close();

    // リセット
    util::software_reset(15);
}

void menu_task::select_key_list(simple_menu::menu *menu, simple_menu::page *page, simple_menu::item *item) {
    // キー一覧
    auto *subpage = new simple_menu::page("Key List");
    auto _manager = key_manager();
    auto *list = _manager.get_list();
    for(int i = 0; i < list->size(); i++) {
        subpage->append(new simple_menu::item(list->get(i), NULL, NULL, menu_task::select_delete_key));
    }
    subpage->append(new simple_menu::back_item());
    menu->push(subpage);
}

void menu_task::select_delete_key(simple_menu::menu *menu, simple_menu::page *page, simple_menu::item *item) {
    // キー削除
    auto *subpage = new simple_menu::page("Delete " + item->title + "?");
    subpage->append(new simple_menu::back_item("NO"));
    subpage->append(new simple_menu::item("YES", item->title.c_str(), NULL, menu_task::remove_key));
    menu->push(subpage);
}

void menu_task::remove_key(simple_menu::menu *menu, simple_menu::page *page, simple_menu::item *item) {
    const char *name = (const char *)item->info;
    Serial.printf("delete key %s\n", name);
    auto manager = key_manager();
    manager.remove(name);
    menu->pop();
    menu->pop(); // 雑
}

void menu_task::select_about(simple_menu::menu *menu, simple_menu::page *page, simple_menu::item *item) {
}

} // namespace task