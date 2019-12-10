/**
 * task_manager.cpp
 */
#include "task_manager.hpp"
#include <Arduino.h>

namespace simple_task {
task_manager::~task_manager() {
    Serial.println("task manager dealloc");
    for(int index = _task_list.size() - 1; index >= 0; index--) {
        task *t = _task_list.get(index);
        _task_list.remove(index);
        delete t;
    }
}

int task_manager::count() { return _task_list.size(); }

void task_manager::add(task *t) {
    _task_list.add(t);
    t->on_attach(this);
}

void task_manager::remove(task *t) {
    for(int index = _task_list.size() - 1; index >= 0; index--) {
        if(t == _task_list.get(index)) {
            t->on_detach(this);
            _task_list.remove(index);
            delete t;
        }
    }
}

task *task_manager::get(int index) { return _task_list.get(index); }

task *task_manager::get_by_id(uint32_t task_id) {
    for(int index = 0; index < _task_list.size(); index++) {
        task *task = _task_list.get(index);
        if(task->get_id() == task_id) {
            return task;
        }
    }
    return NULL;
}

void task_manager::update() {
    for(int index = 0; index < _task_list.size(); index++) {
        task *t = _task_list.get(index);
        t->update();
    }
}
} // namespace simple_task
