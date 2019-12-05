/**
 * page.cpp
 */
#include "page.hpp"

namespace simple_menu {

page::~page() {
    Serial.printf("page dealloc\n");

    for(int index = item_list.size() - 1; index >= 0; index--) {
        item *item = item_list.get(index);
        item_list.remove(index);
        delete item;
    }
}

void page::append(item *item) {
    Serial.printf("add item = %s\n", item->title);
    item_list.add(item);
}

bool page::set_selected_index(int index) {
    Serial.printf("index = %d\n", index);
    Serial.printf("size = %d\n", size());

    if(index == selected_index) {
        return false;
    }

    if(index >= 0 && index < size()) {
        selected_index = index;
        Serial.printf("selected index = %d\n", index);
        return true;
    } else {
        return false;
    }
}

int page::get_selected_index() { return selected_index; }

int page::size() { return item_list.size(); }

item *page::get_item(int index) { return item_list.get(index); }
const char *page::get_title() { return title; }

} // namespace simple_menu