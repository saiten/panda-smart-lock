/**
 * menu_item.hpp
 */
#pragma once

#include <Arduino.h>
#include <cstdio>

namespace simple_menu {
class menu;
class page;

struct item {
    typedef void (*callback_t)(menu *, page *, item *);

    item(String title, page *subpage, callback_t on_selected, callback_t on_entered)
        : title(title), subpage(subpage), on_selected(on_selected), on_entered(on_entered) {}
    String title;
    page *subpage;
    callback_t on_selected;
    callback_t on_entered;
};

struct back_item : public item {
    back_item() : item(String("Back"), NULL, NULL, back_item::_on_entered) {}

  private:
    static void _on_entered(menu *menu, page *, item *item);
};

} // namespace simple_menu
