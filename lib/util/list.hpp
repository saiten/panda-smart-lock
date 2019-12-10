/**
 * list.hpp
 */
#pragma once

#include <cstdio>

namespace util {
template <typename T>
class list {
  public:
    list() : head(nullptr), _size(0) {}

    ~list() {
        for(int index = _size - 1; index > 0; index--) {
            remove(index);
        }
    }

    int size() { return _size; }

    void add(T value) {
        node *n = new node();
        n->value = value;
        n->next = NULL;

        if(!head) {
            head = n;
        } else {
            node *curr = head;
            while(curr->next) {
                curr = curr->next;
            }
            curr->next = n;
        }
        _size++;
    }

    void insert(T value, int index) {
        node *n = new node();
        n->value = value;
        n->next = NULL;

        node *curr = head, *prev = NULL;
        int i = 0;
        while(i++ < index) {
            prev = curr;
            curr = curr->next;
        }
        if(!prev) {
            head = n;
            n->next = curr;
        } else {
            prev->next = n;
            n->next = curr;
        }
        _size++;
    }

    void remove(int index) {
        int i = 0;
        node *curr = head, *prev = NULL;
        while(i++ < index) {
            prev = curr;
            curr = curr->next;
        }
        if(prev) {
            prev->next = curr->next;
        } else {
            head = curr->next;
        }
        if(curr == head) {
            head = NULL;
        }
        delete curr;
        _size--;
    }

    T get(int index) {
        int i = 0;
        node *curr = head;
        while(i++ < index) {
            curr = curr->next;
        }
        return curr->value;
    }

  private:
    struct node {
        T value;
        struct node *next;
    };

    node *head;
    int _size;
    bool _autodelete = false;
};
} // namespace util
