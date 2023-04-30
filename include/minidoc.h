#ifndef MINIDOC_H
#define MINIDOC_H

#include <cstddef> // std::nullopt_t

#include "cache.h"
#include "piece.h"
#include "undo.h"

namespace MiniDoc {
    template <typename T>
    class MiniDoc {
        public:
        class Info {
            friend MiniDoc;
            PieceTable<T> piece;
            T character_;
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
            T character() const;
            size_t cursor() const;
            size_t line_start() const;
            size_t line_end() const;
            size_t line_length() const;
            size_t line() const;
            size_t column() const;
            size_t length() const;
            
            void line_str(std::basic_string<T> & out) const;
            void line_str(std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const;
            std::basic_string<T> line_str() const;
            std::basic_string<T> line_str(std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const;
            
            void str(std::basic_string<T> & out) const;
            void str(std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const;
            std::basic_string<T> str() const;
            std::basic_string<T> str(std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const;
            
            void sub_str(size_t pos, size_t len, std::basic_string<T> & out) const;
            void sub_str(size_t pos, size_t len, std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const;
            std::basic_string<T> sub_str(size_t pos, size_t len) const;
            std::basic_string<T> sub_str(size_t pos, size_t len, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const;
            
            void print(std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;
            void print(const char * indent, std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;
        };
        
        private:
        
        Info info;
        UndoStack<Info> stack = info;
        
        
        public:
        MiniDoc();
        void load(std::nullptr_t stream, size_t length);
        void load(std::nullptr_t stream);
        void load(const T * stream, size_t length);
        void load(const T * stream);
        void seek(size_t pos);
        bool has_next() const;
        T next();
        bool has_previous() const;
        T previous();
        
        T character() const;
        size_t cursor() const;
        size_t line_start() const;
        size_t line_end() const;
        size_t line_length() const;
        size_t line() const;
        size_t column() const;
        size_t length() const;
        
        void line_str(std::basic_string<T> & out) const;
        void line_str(std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const;
        std::basic_string<T> line_str() const;
        std::basic_string<T> line_str(std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const;
        
        void str(std::basic_string<T> & out) const;
        void str(std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const;
        std::basic_string<T> str() const;
        std::basic_string<T> str(std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const;
        
        void sub_str(size_t pos, size_t len, std::basic_string<T> & out) const;
        void sub_str(size_t pos, size_t len, std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const;
        std::basic_string<T> sub_str(size_t pos, size_t len) const;
        std::basic_string<T> sub_str(size_t pos, size_t len, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const;
        
        UndoStackHolder<Info> undoStack();
        
        void append(const T * str);
        void insert(size_t pos, const T * str);
        void replace(size_t pos, const T * str);
        void replace(size_t pos, size_t len, const T * str);
        void erase(size_t pos, size_t len);
        
        void print(std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;
    };
}



// source

#include <minidoc.h>

#include <stdlib.h> // malloc.h is not portable
#include <cstring> // strlen
#include <hexdump.hpp>

namespace MiniDoc {
    template <typename T>
    MiniDoc<T>::MiniDoc() {
        load("");
    }
    
    template <typename T>
    void MiniDoc<T>::load(std::nullptr_t stream) {
        load("");
    }
    
    template <typename T>
    void MiniDoc<T>::load(std::nullptr_t stream, size_t length) {
        load("", length);
    }
    
    template <typename T>
    void MiniDoc<T>::load(const T* stream) {
        load(stream, strlen(stream));
    }
    
    template <typename T>
    void MiniDoc<T>::append(const T * str) {
        insert(-1, str);
    }
    
    template <typename T>
    void MiniDoc<T>::insert(size_t pos, const T * str) {
        stack.push_undo();
        // stay inside bounds 0 to length_
        info.piece.insert(pos == -1 ? info.length_ : pos >= info.length_ ? info.length_ : pos, str);
        info.updateLength();
    }
    
    template <typename T>
    void MiniDoc<T>::replace(size_t pos, const T * str) {
        replace(pos, 1, str);
    }
    
    template <typename T>
    void MiniDoc<T>::replace(size_t pos, size_t len, const T * str) {
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
    
    template <typename T>
    void MiniDoc<T>::erase(size_t pos, size_t len) {
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
    
    template <typename T>
    void MiniDoc<T>::Info::updateLineInfo() {
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
    
    template <typename T>
    void MiniDoc<T>::Info::updateLength() {
        updateLineInfo();
        length_ = piece.length_cached();
        if (length_ > 0) {
            length_--;
        }
    }
    
    template <typename T>
    void MiniDoc<T>::load(const T* stream, size_t length) {
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
    
    template <typename T>
    void MiniDoc<T>::seek(size_t pos) {
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
    
    template <typename T>
    bool MiniDoc<T>::has_next() const {
        return info.cursor_ != info.length_;
    }
    
    template <typename T>
    T MiniDoc<T>::next() {
        T ch = character();
        if (has_next()) {
            info.cursor_++;
            info.updateLineInfo();
        }
        return ch;
    }
    
    template <typename T>
    bool MiniDoc<T>::has_previous() const {
        return info.cursor_ != 0;
    }
    
    template <typename T>
    T MiniDoc<T>::previous() {
        T ch = character();
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
    
    template <typename T>
    void MiniDoc<T>::Info::print(std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        print("", conv);
    }
    
    template <typename T>
    void MiniDoc<T>::Info::print(const char * indent, std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        const T * i = indent == nullptr ? "" : indent;
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
        std::cout << CustomHexdump<8, true, T>("        ", s.c_str(), s.size(), conv) << std::endl;
        int h;
        char c;
        conv(&character_, &h, &c);
        printf("%scharacter:  '%s'\n", i, charToString(c));
    }
    
    template <typename T>
    T MiniDoc<T>::Info::character() const {
        return character_;
    }
    template <typename T>
    size_t MiniDoc<T>::Info::cursor() const {
        return cursor_;
    }
    template <typename T>
    size_t MiniDoc<T>::Info::line_start() const {
        return line_start_;
    }
    template <typename T>
    size_t MiniDoc<T>::Info::line_end() const {
        return line_end_;
    }
    template <typename T>
    size_t MiniDoc<T>::Info::line_length() const {
        return line_length_;
    }
    template <typename T>
    size_t MiniDoc<T>::Info::line() const {
        return line_;
    }
    template <typename T>
    size_t MiniDoc<T>::Info::column() const {
        return column_;
    }
    template <typename T>
    size_t MiniDoc<T>::Info::length() const {
        return length_;
    }
    template <typename T>
    T MiniDoc<T>::character() const {
        return info.character();
    }
    template <typename T>
    size_t MiniDoc<T>::cursor() const {
        return info.cursor();
    }
    template <typename T>
    size_t MiniDoc<T>::line_start() const {
        return info.line_start();
    }
    template <typename T>
    size_t MiniDoc<T>::line_end() const {
        return info.line_end();
    }
    template <typename T>
    size_t MiniDoc<T>::line_length() const {
        return info.line_length();
    }
    template <typename T>
    size_t MiniDoc<T>::line() const {
        return info.line();
    }
    template <typename T>
    size_t MiniDoc<T>::column() const {
        return info.column();
    }
    template <typename T>
    size_t MiniDoc<T>::length() const {
        return info.length();
    }
    
    template <typename T>
    void MiniDoc<T>::Info::line_str(std::basic_string<T> & out) const {
        piece.range_string(line_start_, line_end_, out);
    }
    template <typename T>
    void MiniDoc<T>::Info::line_str(std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        piece.range_string_func(line_start_, line_end_, out, func);
    }
    template <typename T>
    std::basic_string<T> MiniDoc<T>::Info::line_str() const {
        std::basic_string<T> s;
        line_str(s);
        return s;
    }
    template <typename T>
    std::basic_string<T> MiniDoc<T>::Info::line_str(std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        std::basic_string<T> s;
        line_str(s, func);
        return s;
    }
    
    template <typename T>
    void MiniDoc<T>::line_str(std::basic_string<T> & out) const {
        info.line_str(out);
    }
    template <typename T>
    void MiniDoc<T>::line_str(std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        info.line_str(out, func);
    }
    template <typename T>
    std::basic_string<T> MiniDoc<T>::line_str() const {
        std::basic_string<T> s;
        line_str(s);
        return s;
    }
    template <typename T>
    std::basic_string<T> MiniDoc<T>::line_str(std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        std::basic_string<T> s;
        line_str(s, func);
        return s;
    }
    
    template <typename T>
    void MiniDoc<T>::Info::str(std::basic_string<T> & out) const {
        piece.range_string(0, length_, out);
    }
    template <typename T>
    void MiniDoc<T>::Info::str(std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        piece.range_string_func(0, length_, out, func);
    }
    template <typename T>
    std::basic_string<T> MiniDoc<T>::Info::str() const {
        std::basic_string<T> s;
        str(s);
        return s;
    }
    template <typename T>
    std::basic_string<T> MiniDoc<T>::Info::str(std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        std::basic_string<T> s;
        str(s, func);
        return s;
    }
    
    template <typename T>
    void MiniDoc<T>::str(std::basic_string<T> & out) const {
        info.str(out);
    }
    template <typename T>
    void MiniDoc<T>::str(std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        info.str(out, func);
    }
    template <typename T>
    std::basic_string<T> MiniDoc<T>::str() const {
        std::basic_string<T> s;
        str(s);
        return s;
    }
    template <typename T>
    std::basic_string<T> MiniDoc<T>::str(std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        std::basic_string<T> s;
        str(s, func);
        return s;
    }
    
    template <typename T>
    void MiniDoc<T>::Info::sub_str(size_t pos, size_t len, std::basic_string<T> & out) const {
        auto p = pos == -1 ? length_ : pos >= length_ ? length_ : pos;
        piece.range_string(p, p+len, out);
    }
    template <typename T>
    void MiniDoc<T>::Info::sub_str(size_t pos, size_t len, std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        auto p = pos == -1 ? length_ : pos >= length_ ? length_ : pos;
        piece.range_string_func(p, p+len, out, func);
    }
    template <typename T>
    std::basic_string<T> MiniDoc<T>::Info::sub_str(size_t pos, size_t len) const {
        std::basic_string<T> s;
        sub_str(pos, len, s);
        return s;
    }
    template <typename T>
    std::basic_string<T> MiniDoc<T>::Info::sub_str(size_t pos, size_t len, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        std::basic_string<T> s;
        sub_str(pos, len, s, func);
        return s;
    }
    
    template <typename T>
    void MiniDoc<T>::sub_str(size_t pos, size_t len, std::basic_string<T> & out) const {
        info.sub_str(pos, len, out);
    }
    template <typename T>
    void MiniDoc<T>::sub_str(size_t pos, size_t len, std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        info.sub_str(pos, len, out, func);
    }
    template <typename T>
    std::basic_string<T> MiniDoc<T>::sub_str(size_t pos, size_t len) const {
        std::basic_string<T> s;
        sub_str(pos, len, s);
        
        return s;
    }
    template <typename T>
    std::basic_string<T> MiniDoc<T>::sub_str(size_t pos, size_t len, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        std::basic_string<T> s;
        sub_str(pos, len, s, func);
        return s;
    }
    
    template <typename T>
    UndoStackHolder<typename MiniDoc<T>::MiniDoc::Info> MiniDoc<T>::undoStack() {
        return stack;
    }
    
    template <typename T>
    void MiniDoc<T>::print(std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        info.print(conv);
        stack.print([&](const Info & i) { i.print("    ", conv); });
        printf("\n");
    }
}
#endif