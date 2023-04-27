#ifndef MINIDOC_H
#define MINIDOC_H

#include <cstddef> // std::nullopt_t

#include "cache.h"
#include "piece.h"
#include "undo.h"

namespace MiniDoc {
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
      char character() const;
      size_t cursor() const;
      size_t line_start() const;
      size_t line_end() const;
      size_t line_length() const;
      size_t line() const;
      size_t column() const;
      size_t length() const;
      
      void line_str(std::string & out) const;
      void line_str(std::string & out, std::function<void(std::string & out, const char *string, size_t length)> func) const;
      std::string line_str() const;
      std::string line_str(std::function<void(std::string & out, const char *string, size_t length)> func) const;
      
      void str(std::string & out) const;
      void str(std::string & out, std::function<void(std::string & out, const char *string, size_t length)> func) const;
      std::string str() const;
      std::string str(std::function<void(std::string & out, const char *string, size_t length)> func) const;
      
      void sub_str(size_t pos, size_t len, std::string & out) const;
      void sub_str(size_t pos, size_t len, std::string & out, std::function<void(std::string & out, const char *string, size_t length)> func) const;
      std::string sub_str(size_t pos, size_t len) const;
      std::string sub_str(size_t pos, size_t len, std::function<void(std::string & out, const char *string, size_t length)> func) const;
      
      void print() const;
      void print(const char * indent) const;
    };
    
    private:
    
    Info info;
    UndoStack<Info> stack = info;
    
    
    public:
    MiniDoc();
    void load(std::nullptr_t stream, size_t length);
    void load(std::nullptr_t stream);
    void load(const char * stream, size_t length);
    void load(const char * stream);
    void seek(size_t pos);
    bool has_next() const;
    char next();
    bool has_previous() const;
    char previous();
    
    char character() const;
    size_t cursor() const;
    size_t line_start() const;
    size_t line_end() const;
    size_t line_length() const;
    size_t line() const;
    size_t column() const;
    size_t length() const;
    
    void line_str(std::string & out) const;
    void line_str(std::string & out, std::function<void(std::string & out, const char *string, size_t length)> func) const;
    std::string line_str() const;
    std::string line_str(std::function<void(std::string & out, const char *string, size_t length)> func) const;
    
    void str(std::string & out) const;
    void str(std::string & out, std::function<void(std::string & out, const char *string, size_t length)> func) const;
    std::string str() const;
    std::string str(std::function<void(std::string & out, const char *string, size_t length)> func) const;
    
    void sub_str(size_t pos, size_t len, std::string & out) const;
    void sub_str(size_t pos, size_t len, std::string & out, std::function<void(std::string & out, const char *string, size_t length)> func) const;
    std::string sub_str(size_t pos, size_t len) const;
    std::string sub_str(size_t pos, size_t len, std::function<void(std::string & out, const char *string, size_t length)> func) const;
    
    UndoStackHolder<Info> undoStack();
    
    void append(const char * str);
    void insert(size_t pos, const char * str);
    void replace(size_t pos, const char * str);
    void replace(size_t pos, size_t len, const char * str);
    void erase(size_t pos, size_t len);
    
    void print() const;
  };
}
#endif