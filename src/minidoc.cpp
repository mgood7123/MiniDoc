#include <minidoc.h>

#include <stdlib.h> // malloc.h is not portable
#include <cstring> // strlen

namespace MiniDoc {
  
  void MiniDoc::push_undo() {
    undo_stack.push_back(info);
    if (redo_stack.size() != 0) {
      undo_stack.insert(undo_stack.end(), redo_stack.rbegin(), redo_stack.rend()-1);
      undo_stack.insert(undo_stack.end(), redo_stack.begin(), redo_stack.end());
      redo_stack = {};
      undo_stack.push_back(info);
    }
  }
  
  bool MiniDoc::undo() {
    if (undo_stack.size() == 0) {
      return false;
    }
    auto & p = undo_stack.back();
    redo_stack.push_back(info);
    info = p;
    undo_stack.pop_back();
    return true;
  }
  
  bool MiniDoc::redo() {
    if (redo_stack.size() == 0) {
      return false;
    }
    auto & p = redo_stack.back();
    undo_stack.push_back(info);
    info = p;
    redo_stack.pop_back();
    return true;
  }
  
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
    push_undo();
    info.piece.insert(pos == -1 ? info.length_ : pos >= info.length_ ? info.length_ : pos, str);
    info.updateLength();
  }
  
  void MiniDoc::replace(size_t pos, const char * str) {
    replace(pos, 1, str);
  }
  
  void MiniDoc::replace(size_t pos, size_t len, const char * str) {
    push_undo();
    auto p = pos == -1 ? info.length_ : pos >= info.length_ ? info.length_ : pos;
    info.piece.erase(p, p+len);
    info.piece.insert(p, str);
    info.updateLength();
  }
  
  void MiniDoc::erase(size_t pos, size_t len) {
    push_undo();
    auto p = pos == -1 ? info.length_ : pos >= info.length_ ? info.length_ : pos;
    info.piece.erase(p, p+len);
    info.updateLength();
  }
  
  void MiniDoc::Info::updateLineInfo() {
    line_ = piece.get_line(cursor_);
    line_start_ = piece.line_start(line_);
    line_end_ = piece.line_end(line_);
    line_length_ = line_end_ - line_start_;
    if (line_ == piece.lines()) {
      line_end_--;
      line_length_--;
    }
    column_ = cursor_ - line_start_;
    character_ = piece[cursor_];
  }
  
  void MiniDoc::Info::updateLength() {
    updateLineInfo();
    length_ = piece.length();
    if (length_ > 0) {
      length_--;
    }
  }
  
  void MiniDoc::load(const char* stream, size_t length) {
    info = {};
    resetUndoRedoStack();
    
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
  
  bool MiniDoc::hasNext() {
    return info.cursor_ <= info.length_;
  }
  
  bool MiniDoc::next() {
    if (hasNext()) {
      info.cursor_++;
      info.updateLineInfo();
      return true;
    }
    return false;
  }
  
  bool MiniDoc::hasPrevious() {
    return info.cursor_ != 0;
  }
  
  bool MiniDoc::previous() {
    if (hasPrevious()) {
      info.cursor_--;
      info.updateLineInfo();
      return true;
    }
    return false;
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
  
  void MiniDoc::Info::print() {
    print("");
  }
  
  void MiniDoc::Info::print(const char * indent) {
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
    printf("%scharacter:  '%s'\n", i, charToString(character_));
  }
  
  char MiniDoc::Info::character() {
    return character_;
  }
  size_t MiniDoc::Info::cursor() {
    return cursor_;
  }
  size_t MiniDoc::Info::line_start(){
    return line_start_;
  }
  size_t MiniDoc::Info::line_end(){
    return line_end_;
  }
  size_t MiniDoc::Info::line_length(){
    return line_length_;
  }
  size_t MiniDoc::Info::line(){
    return line_;
  }
  size_t MiniDoc::Info::column(){
    return column_;
  }
  size_t MiniDoc::Info::length(){
    return length_;
  }
  char MiniDoc::character() {
    return info.character();
  }
  size_t MiniDoc::cursor() {
    return info.cursor();
  }
  size_t MiniDoc::line_start(){
    return info.line_start();
  }
  size_t MiniDoc::line_end(){
    return info.line_end();
  }
  size_t MiniDoc::line_length(){
    return info.line_length();
  }
  size_t MiniDoc::line(){
    return info.line();
  }
  size_t MiniDoc::column(){
    return info.column();
  }
  size_t MiniDoc::length(){
    return info.length();
  }
  
  std::string MiniDoc::Info::line_str() {
    return piece.range_string(line_start_, line_end_);
  }
  std::string MiniDoc::line_str() {
    return info.line_str();
  }
  
  std::string MiniDoc::Info::str() {
    return piece.range_string(0, length_);
  }
  std::string MiniDoc::str() {
    return info.str();
  }
  
  std::string MiniDoc::Info::sub_str(size_t pos, size_t len) {
    auto p = pos == -1 ? length_ : pos >= length_ ? length_ : pos;
    return piece.range_string(p, p+len);
  }
  std::string MiniDoc::sub_str(size_t pos, size_t len) {
    return info.sub_str(pos, len);
  }
  
  void MiniDoc::resetUndoRedoStack() {
    undo_stack = {};
    redo_stack = {};
  }
  
  void MiniDoc::print() {
    info.print();
    size_t s;
    printf("undo stack: %zu items\n", undo_stack.size());
    if (undo_stack.size() != 0) {
      s = 0;
      for (auto & i : undo_stack) {
        printf("\n  undo #%zu\n", s);
        i.print("    ");
        s++;
      }
      printf("\n\n");
    }
    printf("redo stack: %zu items\n", redo_stack.size());
    if (redo_stack.size() != 0) {
      s = 0;
      for (auto & i : redo_stack) {
        printf("\n  redo #%zu\n", s);
        i.print("    ");
        s++;
      }
      printf("\n\n");
    }
    printf("\n");
  }
}