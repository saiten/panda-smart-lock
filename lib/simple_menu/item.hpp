/**
 * menu_item.hpp
 */
#pragma once

#include <cstdio>

namespace simple_menu {
class menu;
class page;

struct item {
    typedef void (*callback)(menu *, page *, item *);

    item(const char *title, page *subpage, callback on_selected, callback on_entered)
        : title(title), subpage(subpage), on_selected(on_selected), on_entered(on_entered) {}
    const char *title;
    page *subpage;
    callback on_selected;
    callback on_entered;
};

struct back_item : public item {
    back_item() : item("Back", NULL, NULL, back_item::_on_entered) {}

  private:
    static void _on_entered(menu *menu, page *, item *item);
};

} // namespace simple_menu
