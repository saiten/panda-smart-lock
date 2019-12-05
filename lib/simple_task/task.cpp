/**
 * task.cpp
 */
#include "task.hpp"
#include "task_manager.hpp"

namespace simple_task {

void task::on_attach(task_manager *manager) {
    _manager = manager;
    _attached_time = millis();
}

void task::on_detach(task_manager *manager) {
    _manager = NULL;
    _attached_time = 0;
}

void task::destroy() {
    if(_manager != NULL) {
        _manager->remove(this);
    }
}

} // namespace simple_task
