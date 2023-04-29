#include <minidoc.h>

#include <stdlib.h> // malloc.h is not portable
#include <cstring> // strlen
#include <hexdump.hpp>

namespace MiniDoc {
  
  MiniDoc::MiniDoc() {
    load("");
  }
  
  void MiniDoc::load(std::nullptr_t stream) {
    load("");
  }
  
  void MiniDoc::load(std::nullptr_t stream, size_t length) {
    load("", length);
  }
  
  void MiniDoc::load(const char* stream) {
    load(stream, strlen(stream));
  }
  
  void MiniDoc::append(const char * str) {
    insert(-1, str);
  }
  
  void MiniDoc::insert(size_t pos, const char * str) {
    stack.push_undo();
    // stay inside bounds 0 to length_
    info.piece.insert(pos == -1 ? info.length_ : pos >= info.length_ ? info.length_ : pos, str);
    info.updateLength();
  }
  
  void MiniDoc::replace(size_t pos, const char * str) {
    replace(pos, 1, str);
  }
  
  void MiniDoc::replace(size_t pos, size_t len, const char * str) {
    stack.push_undo();
    // stay inside bounds 0 to length_
    size_t p = pos < 0 ? info.length_ : pos >= info.length_ ? info.length_ : pos;
    if (p != info.length_) {
      // stay inside bounds 0 to length_
      size_t l = len == -1 ? info.length_ : p + len >= info.length_ ? info.length_ : p + len;
      if (l != 0) {
        info.piece.erase(p, l);
      }
    }
    info.piece.insert(p, str);
    info.updateLength();
  }
  
  void MiniDoc::erase(size_t pos, size_t len) {
    stack.push_undo();
    
    // stay inside bounds 0 to length_
    size_t p = pos == -1 ? info.length_ : pos >= info.length_ ? info.length_ : pos;
    if (p != info.length_) {
        
      // stay inside bounds 0 to length_
      size_t l = len == -1 ? info.length_ : p + len >= info.length_ ? info.length_ : p + len;
      if (l != 0) {
        info.piece.erase(p, l);
        info.updateLength();
      }
    }
  }
  
  void MiniDoc::Info::updateLineInfo() {
    line_ = piece.get_line(cursor_);
    line_start_ = piece.line_start_cached(line_);
    line_end_ = piece.line_end_cached(line_);
    line_length_ = line_end_ - line_start_;
    if (line_ == piece.lines_cached()) {
      line_end_--;
      line_length_--;
    }
    column_ = cursor_ - line_start_;
    character_ = piece[cursor_];
  }
  
  void MiniDoc::Info::updateLength() {
    updateLineInfo();
    length_ = piece.length_cached();
    if (length_ > 0) {
      length_--;
    }
  }
  
  void MiniDoc::load(const char* stream, size_t length) {
    info = {};
    stack.reset();
    
    if (length == 0) {
      info.piece.append_origin("\n", 1);
    } else {
      info.piece.append_origin(stream, length);
      if (length == 0 || stream[length-1] != '\n') {
        info.piece.append_origin("\n", 1);
      }
    }
    info.updateLength();
  }
  
  void MiniDoc::seek(size_t pos) {
    if (pos == info.cursor_) {
      return;
    }
    if (pos == -1) {
      pos = info.length_;
    }
    if (pos > info.length_) {
      pos = info.length_;
    }
    if (pos > info.cursor_) {
      while (pos != info.cursor_) {
        next();
      }
    } else {
      while (pos != info.cursor_) {
        previous();
      }
    }
  }
  
  bool MiniDoc::has_next() const {
    return info.cursor_ != info.length_;
  }
  
  char MiniDoc::next() {
    char ch = character();
    if (has_next()) {
      info.cursor_++;
      info.updateLineInfo();
    }
    return ch;
  }
  
  bool MiniDoc::has_previous() const {
    return info.cursor_ != 0;
  }
  
  char MiniDoc::previous() {
    char ch = character();
    if (has_previous()) {
      info.cursor_--;
      info.updateLineInfo();
    }
    return ch;
  }
  
  char tmp[2];
  
  const char* charToString(char c) {
    if (c == '\n') {
      return "<newline>";
    } else if (c == '\t') {
      return "<tab>";
    } else if (c == '\r') {
      return "<carrage return>";
    } else if (c == '\0') {
      return "<null>";
    } else {
      tmp[0] = c;
      tmp[1] = '\0';
      return tmp;
    }
  }
  
  void MiniDoc::Info::print() const {
    print("");
  }
  
  void MiniDoc::Info::print(const char * indent) const {
    const char * i = indent == nullptr ? "" : indent;
    printf("%slength: %zu\n", i, length_);
    printf("%scursor: %zu\n", i, cursor_);
    printf("%sline start: %zu\n", i, line_start_);
    printf("%sline end: %zu\n", i, line_end_);
    printf("%sline length: %zu\n", i, line_length_);
    printf("%sline: %zu\n", i, line_);
    printf("%scolumn: %zu\n", i, column_);
    auto s = line_str();
    printf("%sline str: %s\n", i, s.c_str());
    printf("%sline str size: %zu\n", i, s.size());
    printf("%sline str len: %zu\n", i, s.length());
    printf("%sline str hex\n", i);
    std::cout << CustomHexdump<8, true>("        ", s.c_str(), s.size()) << std::endl;
    printf("%scharacter:  '%s'\n", i, charToString(character_));
  }
  
  char MiniDoc::Info::character() const {
    return character_;
  }
  size_t MiniDoc::Info::cursor() const {
    return cursor_;
  }
  size_t MiniDoc::Info::line_start() const {
    return line_start_;
  }
  size_t MiniDoc::Info::line_end() const {
    return line_end_;
  }
  size_t MiniDoc::Info::line_length() const {
    return line_length_;
  }
  size_t MiniDoc::Info::line() const {
    return line_;
  }
  size_t MiniDoc::Info::column() const {
    return column_;
  }
  size_t MiniDoc::Info::length() const {
    return length_;
  }
  char MiniDoc::character() const {
    return info.character();
  }
  size_t MiniDoc::cursor() const {
    return info.cursor();
  }
  size_t MiniDoc::line_start() const {
    return info.line_start();
  }
  size_t MiniDoc::line_end() const {
    return info.line_end();
  }
  size_t MiniDoc::line_length() const {
    return info.line_length();
  }
  size_t MiniDoc::line() const {
    return info.line();
  }
  size_t MiniDoc::column() const {
    return info.column();
  }
  size_t MiniDoc::length() const {
    return info.length();
  }
  
  void MiniDoc::Info::line_str(std::string & out) const {
    piece.range_string(line_start_, line_end_, out);
  }
  void MiniDoc::Info::line_str(std::string & out, std::function<void(std::string & out, const char *string, size_t length)> func) const {
    piece.range_string_func(line_start_, line_end_, out, func);
  }
  std::string MiniDoc::Info::line_str() const {
    std::string s;
    line_str(s);
    return s;
  }
  std::string MiniDoc::Info::line_str(std::function<void(std::string & out, const char *string, size_t length)> func) const {
    std::string s;
    line_str(s, func);
    return s;
  }
  
  void MiniDoc::line_str(std::string & out) const {
    info.line_str(out);
  }
  void MiniDoc::line_str(std::string & out, std::function<void(std::string & out, const char *string, size_t length)> func) const {
    info.line_str(out, func);
  }
  std::string MiniDoc::line_str() const {
    std::string s;
    line_str(s);
    return s;
  }
  std::string MiniDoc::line_str(std::function<void(std::string & out, const char *string, size_t length)> func) const {
    std::string s;
    line_str(s, func);
    return s;
  }
  
  void MiniDoc::Info::str(std::string & out) const {
    piece.range_string(0, length_, out);
  }
  void MiniDoc::Info::str(std::string & out, std::function<void(std::string & out, const char *string, size_t length)> func) const {
    piece.range_string_func(0, length_, out, func);
  }
  std::string MiniDoc::Info::str() const {
    std::string s;
    str(s);
    return s;
  }
  std::string MiniDoc::Info::str(std::function<void(std::string & out, const char *string, size_t length)> func) const {
    std::string s;
    str(s, func);
    return s;
  }
  
  void MiniDoc::str(std::string & out) const {
    info.str(out);
  }
  void MiniDoc::str(std::string & out, std::function<void(std::string & out, const char *string, size_t length)> func) const {
    info.str(out, func);
  }
  std::string MiniDoc::str() const {
    std::string s;
    str(s);
    return s;
  }
  std::string MiniDoc::str(std::function<void(std::string & out, const char *string, size_t length)> func) const {
    std::string s;
    str(s, func);
    return s;
  }
  
  void MiniDoc::Info::sub_str(size_t pos, size_t len, std::string & out) const {
    auto p = pos == -1 ? length_ : pos >= length_ ? length_ : pos;
    piece.range_string(p, p+len, out);
  }
  void MiniDoc::Info::sub_str(size_t pos, size_t len, std::string & out, std::function<void(std::string & out, const char *string, size_t length)> func) const {
    auto p = pos == -1 ? length_ : pos >= length_ ? length_ : pos;
    piece.range_string_func(p, p+len, out, func);
  }
  std::string MiniDoc::Info::sub_str(size_t pos, size_t len) const {
    std::string s;
    sub_str(pos, len, s);
    return s;
  }
  std::string MiniDoc::Info::sub_str(size_t pos, size_t len, std::function<void(std::string & out, const char *string, size_t length)> func) const {
    std::string s;
    sub_str(pos, len, s, func);
    return s;
  }
  
  void MiniDoc::sub_str(size_t pos, size_t len, std::string & out) const {
    info.sub_str(pos, len, out);
  }
  void MiniDoc::sub_str(size_t pos, size_t len, std::string & out, std::function<void(std::string & out, const char *string, size_t length)> func) const {
    info.sub_str(pos, len, out, func);
  }
  std::string MiniDoc::sub_str(size_t pos, size_t len) const {
    std::string s;
    sub_str(pos, len, s);
    return s;
  }
  std::string MiniDoc::sub_str(size_t pos, size_t len, std::function<void(std::string & out, const char *string, size_t length)> func) const {
    std::string s;
    sub_str(pos, len, s, func);
    return s;
  }
  
  UndoStackHolder<MiniDoc::MiniDoc::Info> MiniDoc::undoStack() {
    return stack;
  }
  
  void MiniDoc::print() const {
    info.print();
    stack.print([](const Info & i) { i.print("    "); });
    printf("\n");
  }
}