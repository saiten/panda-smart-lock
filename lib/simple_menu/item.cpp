#include "item.hpp"
#include "menu.hpp"

namespace simple_menu {
void back_item::_on_entered(menu *menu, page *page, item *item) {
    Serial.println("back_item on entered");
    menu->pop();
}
} // namespace simple_menu
