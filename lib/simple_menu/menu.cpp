/**
 * menu.cpp
 */
#include "menu.hpp"

namespace simple_menu {

menu::~menu() {
    Serial.printf("menu dealloc\n");

    for(int index = page_list.size() - 1; index >= 0; index--) {
        page *page = page_list.get(index);
        page_list.remove(index);
        delete page;
    }
}

void menu::push(page *page) {
    Serial.printf("push page = %s\n", page->get_title().c_str());
    page_list.add(page);
    page->set_selected_index(0);
}

page *menu::get_current_page() {
    int page_index = page_list.size() - 1;
    if(page_index < 0) {
        return NULL;
    }
    return page_list.get(page_index);
}

void menu::display() {
    page *page = get_current_page();

    // clear
    _display->clearDisplay();

    // setup
    _display->setTextSize(1);
    _display->setTextColor(WHITE);

    // draw title
    _display->fillRect(0, 0, 128, 7, WHITE);
    _display->setTextColor(BLACK);
    _display->setCursor(4, 0);
    _display->print(page->get_title());
    _display->setTextColor(WHITE);

    const int LINE_NUM = 3;
    int item_size = page->size();
    int page_num = page->get_selected_index() / LINE_NUM;

    for(int offset = 0; offset < LINE_NUM; offset++) {
        int index = page_num * LINE_NUM + offset;
        if(index < item_size) {
            item *item = page->get_item(index);
            int y = 8 + (offset * 8);
            if(page->get_selected_index() == index) {
                _display->setCursor(0, y);
                _display->print(">");
            }
            _display->setCursor(8, y);
            _display->print(item->title);
        }
    }
    _display->display();
}

void menu::set_select_index(int index) {
    page *page = get_current_page();

    if(page->set_selected_index(index)) {
        item *item = this->get_selected_item();
        if(item != NULL && item->on_selected) {
            (*item->on_selected)(this, page, item);
        }
    }
}

void menu::up() {
    Serial.println("menu up");
    page *page = get_current_page();
    int index = page->get_selected_index() - 1;
    set_select_index(index);
}

void menu::down() {
    Serial.println("menu down");
    page *page = get_current_page();
    int index = page->get_selected_index() + 1;
    set_select_index(index);
}

void menu::enter() {
    page *page = get_current_page();
    item *item = this->get_selected_item();
    if(item != NULL) {
        Serial.println("item entered");
        Serial.println(item->title);
        if(item->on_entered) {
            (*item->on_entered)(this, page, item);
        }
    }
}

void menu::pop() {
    Serial.println("menu back");
    int size = page_list.size();
    if(size > 0) {
        auto *page = page_list.get(size - 1);
        page_list.remove(size - 1);
        delete page;
    }
}

int menu::size() { return page_list.size(); }

item *menu::get_selected_item() {
    page *page = get_current_page();
    return page->get_item(page->get_selected_index());
}

} // namespace simple_menu
