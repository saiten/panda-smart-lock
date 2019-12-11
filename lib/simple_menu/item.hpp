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

    item(String title, const void *info, callback_t on_selected, callback_t on_entered)
        : title(title), info(info), on_selected(on_selected), on_entered(on_entered) {}
    String title;
    const void *info;
    callback_t on_selected;
    callback_t on_entered;
};

struct back_item : public item {
    back_item(String title) : item(title, NULL, NULL, back_item::_on_entered) {}
    back_item() : back_item("Back") {}

  private:
    static void _on_entered(menu *menu, page *, item *item);
};

} // namespace simple_menu
