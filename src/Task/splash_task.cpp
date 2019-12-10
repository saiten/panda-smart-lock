/**
 * splash_task.cpp
 */
#include "task/splash_task.hpp"
#include "fiatlogo.h"
#include "task/advertisement_task.hpp"
#include "task/menu_task.hpp"

namespace task {
void splash_task::update() {
    if(millis() - _attached_time > 2000) {
        destroy();
    }
}

void splash_task::on_attach(simple_task::task_manager *manager) {
    simple_task::task::on_attach(manager);

    // splash logo
    display->clearDisplay();
    display->drawXBitmap((display->width() - fiatlogo_width) / 2,
                         (display->height() - fiatlogo_height) / 2,
                         fiatlogo_bits, fiatlogo_width, fiatlogo_height, 1);
    display->display();
}
void splash_task::on_detach(simple_task::task_manager *manager) {
    simple_task::task::on_detach(manager);

    menu_task *task = new menu_task(display);
    manager->add(task);
    advertisement_task *adv_task = new advertisement_task();
    manager->add(adv_task);
}

} // namespace task
