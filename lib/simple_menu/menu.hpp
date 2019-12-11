/**
 * menu.hpp
 */
#pragma once

#include "page.hpp"
#include <Adafruit_SSD1306.h>
#include <cstdio>
#include <util.h>

namespace simple_menu {

class menu {
  public:
    menu(Adafruit_SSD1306 *display)
        : _display(display), page_list(util::list<page *>()) {}
    ~menu();

    void display();

    void set_select_index(int index);
    void up();
    void down();
    void enter();

    void swap(page *page);
    void push(page *page);
    void pop();

    int size();

  protected:
    Adafruit_SSD1306 *_display = NULL;
    util::list<page *> page_list;
    page *get_current_page();
    item *get_selected_item();
};

} // namespace simple_menu
