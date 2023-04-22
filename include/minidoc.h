#ifndef MINIDOC_H
#define MINIDOC_H

#include <stdio.h>
#include <cstddef> // std::nullopt_t

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include <iostream>
#include <set>
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include <deque>

#ifdef GTEST_API_
#define MINIDOC_GTEST public:
#else
#define MINIDOC_GTEST private:
#endif

namespace MiniDoc {
  
  #include "piece.h"
  
  class MiniDoc {
    public:
    class Info {
      friend MiniDoc;
      PieceTable<char> piece;
      char character_ = '\0';
      size_t cursor_ = 0;
      size_t line_ = 0;
      size_t line_start_ = 0;
      size_t line_end_ = 0;
      size_t line_length_ = 0;
      size_t column_ = 0;
      size_t length_ = 0;
      
      void updateLineInfo();
      void updateLength();
      
      public:
      char character();
      size_t cursor();
      size_t line_start();
      size_t line_end();
      size_t line_length();
      size_t line();
      size_t column();
      size_t length();
      std::string line_str();
      std::string str();
      std::string sub_str(size_t pos, size_t len);
      void print();
      void print(const char * indent);
    };
    
    MINIDOC_GTEST
    
    Info info;
    
    // undo/redo stack
    // supports advanced undo/redo
    std::deque<Info> undo_stack;
    std::deque<Info> redo_stack;
    
    void push_undo();
    
    
    public:
    bool undo();
    bool redo();
    MiniDoc();
    void load(std::nullptr_t stream, size_t length);
    void load(std::nullptr_t stream);
    void load(const char * stream, size_t length);
    void load(const char * stream);
    void seek(size_t pos);
    bool hasNext();
    bool next();
    bool hasPrevious();
    bool previous();
    
    char character();
    size_t cursor();
    size_t line_start();
    size_t line_end();
    size_t line_length();
    size_t line();
    size_t column();
    size_t length();
    std::string line_str();
    std::string str();
    std::string sub_str(size_t pos, size_t len);
    
    void resetUndoRedoStack();
    
    void append(const char * str);
    void insert(size_t pos, const char * str);
    void replace(size_t pos, const char * str);
    void replace(size_t pos, size_t len, const char * str);
    void erase(size_t pos, size_t len);
    
    void print();
    
  };
}
#endif