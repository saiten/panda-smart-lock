/**
 * shared_ptr.hpp
 */
#pragma once

#include <cassert>

namespace util {
template <typename T>
class shared_ptr {
  public:
    explicit inline shared_ptr(T *ptr) : _ptr(ptr), _ref_count(new int(1)) {}
    inline shared_ptr(const shared_ptr<T> &rhs)
        : _ptr(rhs._ptr), _ref_count(rhs._ref_count) {
        assert(_ref_count);
        increment();
    }

    inline ~shared_ptr() {
        assert(_ref_count);
        assert(*_ref_count >= 1);
        if(decrement() == 0) {
            free();
        }
    }

    T &operator*() { return *_ptr; }
    const T &operator*() const { return *_ptr; }
    T *operator->() { return _ptr; }
    const T *operator->() const { return _ptr; }
    operator bool() { return _ptr; }
    shared_ptr<T> &operator=(const shared_ptr<T> &rhs) {
        if(this != &rhs) {
            if(decrement() == 0) {
                free();
            }
            _ptr = rhs._ptr;
            _ref_count = rhs._ref_count;
            assert(_ref_count);
            increment();
        }
        return *this;
    }

  private:
    T *_ptr;
    int *_ref_count;

    void free() {
        if(_ptr) {
            delete _ptr;
            _ptr = NULL;
        }
        delete _ref_count;
        _ref_count = NULL;
    }

    void increment() { ++(*_ref_count); }
    int decrement() { return --(*_ref_count); }
};
} // namespace util