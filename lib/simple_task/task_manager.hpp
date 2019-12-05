/**
 * task_manager.hpp
 */
#pragma once

#include "task.hpp"
#include <util.h>

namespace simple_task {
class task_manager {
  public:
    ~task_manager();
    int count();
    void add(task *t);
    void remove(task *t);
    task *get(int index);
    void update();

  private:
    util::list<task *> _task_list;
};
} // namespace simple_task