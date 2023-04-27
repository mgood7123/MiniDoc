#ifndef MINIDOC_UNDO_H
#define MINIDOC_UNDO_H

#include <deque>
#include <functional>
#include <stdio.h>

namespace MiniDoc {
  template<typename T>
  class UndoStack {
    private:
    std::deque<T> undo_stack;
    std::deque<T> redo_stack;
    T * ref;
    
    public:
    
    bool supports_redo = true;
    bool supports_advanced_undo = true;
    
    UndoStack(T & ref) : ref(&ref) {};
    
    void push_undo() {
      undo_stack.push_back(*ref);
      if (supports_redo) {
        if (!supports_advanced_undo) {
          redo_stack = {};
        } else if (redo_stack.size() != 0) {
          undo_stack.insert(undo_stack.end(), redo_stack.rbegin(), redo_stack.rend()-1);
          undo_stack.insert(undo_stack.end(), redo_stack.begin(), redo_stack.end());
          redo_stack = {};
          undo_stack.push_back(*ref);
        }
      }
    }
    
    bool undo() {
      if (undo_stack.size() == 0) {
        return false;
      }
      auto & p = undo_stack.back();
      if (supports_advanced_undo) {
        redo_stack.push_back(*ref);
      }
      *ref = p;
      undo_stack.pop_back();
      return true;
    }
    
    bool redo() {
      if (!supports_redo || redo_stack.size() == 0) {
        return false;
      }
      auto & p = redo_stack.back();
      undo_stack.push_back(*ref);
      *ref = p;
      redo_stack.pop_back();
      return true;
    }
    
    void reset() {
      undo_stack = {};
      redo_stack = {};
    }
    
    void print(const std::function<void(const T&)> & printer) const {
      size_t s;
      printf("undo stack: %zu items\n", undo_stack.size());
      if (undo_stack.size() != 0) {
        s = 0;
        for (auto & i : undo_stack) {
          printf("\n  undo #%zu\n", s);
          printer(i);
          s++;
        }
        printf("\n\n");
      }
      printf("redo stack: %zu items\n", redo_stack.size());
      if (redo_stack.size() != 0) {
        s = 0;
        for (auto & i : redo_stack) {
          printf("\n  redo #%zu\n", s);
          printer(i);
          s++;
        }
        printf("\n\n");
      }
    }
  };
  
  template<typename T>
  class UndoStackHolder {
    UndoStack<T> *ref;
    
    public:
    UndoStackHolder(UndoStack<T> & ref) : ref(&ref), supports_redo(ref.supports_redo), supports_advanced_undo(ref.supports_advanced_undo) {}
    
    // these are required to prevent the following:
    //   auto & s = ...
    //   s = s; // error
    //   s = info; // error
    //   s = {info}; // error
    //   s = UndoStackHolder(info); // error
    
    UndoStackHolder(const UndoStackHolder<T> & other) = delete;
    
    UndoStackHolder(UndoStackHolder<T> && other) = delete;
    
    UndoStackHolder & operator=(const UndoStackHolder<T> & other) = delete;
    
    UndoStackHolder & operator=(UndoStackHolder<T> && other) = delete;
    
    bool & supports_redo;
    bool & supports_advanced_undo;
    
    void push_undo() {
      ref->push_undo();
    }
    void undo() {
      ref->undo();
    }
    void redo() {
      ref->redo();
    }
    
    void reset() {
      ref->reset();
    }
    
    void print(const std::function<void(const T&)> & printer) const {
      ref->print(printer);
    }
  };
}
#endif