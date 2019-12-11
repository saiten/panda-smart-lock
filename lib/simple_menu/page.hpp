/**
 * page.hpp
 */
#pragma once

#include "Arduino.h"
#include "item.hpp"
#include <cstdio>
#include <util.h>

namespace simple_menu {
class page {
  public:
    page(String title)
        : title(title), item_list(util::list<item *>()) {}
    ~page();

    void append(item *item);
    void remove(item *item);
    bool set_selected_index(int index);
    int get_selected_index();
    int size();
    item *get_item(int index);
    String get_title();

  private:
    String title;
    util::list<item *> item_list;
    int selected_index = 0;
};
} // namespace simple_menu