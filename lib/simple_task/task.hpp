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
    virtual ~task() {}
    virtual void update() {}
    virtual void on_attach(task_manager *manager);
    virtual void on_detach(task_manager *manager);

  protected:
    task_manager *_manager = NULL;
    uint32_t _attached_time;

    virtual void destroy();
};
} // namespace simple_task
