/**
 * task.cpp
 */
#include "task.hpp"
#include "task_manager.hpp"

namespace simple_task {

task::task(uint32_t task_id) : _task_id(task_id) {
    Serial.printf("construction task : %d\n", _task_id);
}

task::~task() {
    Serial.printf("destruction task : %d\n", _task_id);
}

void task::on_attach(task_manager *manager) {
    Serial.printf("attach task : %d\n", _task_id);
    _manager = manager;
    _attached_time = millis();
}

void task::on_detach(task_manager *manager) {
    Serial.printf("detach task : %d\n", _task_id);
    _manager = NULL;
    _attached_time = 0;
}

void task::destroy() {
    if(_manager != NULL) {
        _manager->remove(this);
    }
}

uint32_t task::get_id() {
    return _task_id;
}

void task::switch_task(task *task) {
    _manager->add(task);
    destroy();
}

} // namespace simple_task
