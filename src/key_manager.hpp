/**
 * key_manager.hpp
 */
#pragma once

#include "bluefruit.h"
#include <util.h>

class key_manager {
  public:
    key_manager(uint16_t key_size) : key_size(key_size){};
    ~key_manager(){};

    err_t load(const char *name, uint8_t *public_key);
    err_t save(const char *name, const uint8_t *public_key);
    err_t remove(const char *name);

    util::list<String> *get_list();

  private:
    uint16_t key_size;
};