/**
 * task.hpp
 */
#pragma once

#include <Arduino.h>
#include <cstdio>
#include <util.h>

namespace simple_task {
class task_manager;
class task {
  public:
    task(uint32_t task_id);
    virtual ~task();
    virtual void update() {}
    virtual void on_attach(task_manager *manager);
    virtual void on_detach(task_manager *manager);
    virtual uint32_t get_id();
    virtual void switch_task(task *task);
    virtual void destroy();

  protected:
    task_manager *_manager = NULL;
    uint32_t _task_id;
    uint32_t _attached_time;
};
} // namespace simple_task
