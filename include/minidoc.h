#ifndef MINIDOC_H
#define MINIDOC_H

#include <cstddef> // std::nullopt_t

#include "cache.h"
#include "piece.h"
#include "undo.h"

namespace MiniDoc {
    template <typename T, T new_line>
    class MiniDoc {
        T empty[0];
        T new_line_str[1] { new_line };
        public:
        class Info {
            friend MiniDoc;
            PieceTable<T, new_line> piece;
            std::string tag;
            T character_;
            size_t cursor_ = 0;
            size_t line_ = 0;
            size_t line_start_ = 0;
            size_t line_end_ = 0;
            size_t line_length_ = 0;
            size_t column_ = 0;
            size_t length_ = 0;
            
            // backtrace info
            // op 0 = nothing
            // op 1 = insert
            // op 2 = replace
            // op 3 = erase
            uint8_t bt_op = 0;
            size_t bt_pos = 0;
            size_t bt_len = 0;
            
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
            
            uint8_t btop() const;
            size_t btpos() const;
            size_t btlen() const;
            
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
        uint8_t btop() const;
        size_t btpos() const;
        size_t btlen() const;
        
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
        void append(const std::string & undo_tag, const T * str);
        void insert(size_t pos, const T * str);
        void insert(const std::string & undo_tag, size_t pos, const T * str);
        void replace(size_t pos, const T * str);
        void replace(const std::string & undo_tag, size_t pos, const T * str);
        void replace(size_t pos, size_t len, const T * str);
        void replace(const std::string & undo_tag, size_t pos, size_t len, const T * str);
        void erase(size_t pos, size_t len);
        void erase(const std::string & undo_tag, size_t pos, size_t len);
        
        void print(std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;
    };
}



// source>

#include <stdlib.h> // malloc.h is not portable
#include <cstring> // strlen
#include <hexdump.hpp>

namespace MiniDoc {
    template <typename T, T new_line>
    MiniDoc<T, new_line>::MiniDoc() {
        load(empty);
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::load(std::nullptr_t stream) {
        load(empty, 0);
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::load(std::nullptr_t stream, size_t length) {
        load(empty, length);
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::load(const T* stream) {
        load(stream, strlen(stream));
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::load(const T* stream, size_t length) {
        info = {};
        stack.reset();
        
        if (length == 0) {
            info.piece.append_origin(new_line_str, 1);
        } else {
            info.piece.append_origin(stream, length);
            if (length == 0 || stream[length-1] != new_line) {
                info.piece.append_origin(new_line_str, 1);
            }
        }
        info.updateLength();
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::append(const T * str) {
        append(std::string("append ") + str, str);
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::append(const std::string & undo_tag, const T * str) {
        insert(undo_tag, -1, str);
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::insert(size_t pos, const T * str) {
        insert(std::string("insert ") + str + " at position " + std::to_string(pos), pos, str);
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::insert(const std::string & undo_tag, size_t pos, const T * str) {
        info.tag = undo_tag;
        
        // stay inside bounds 0 to length_
        size_t p = pos == -1 ? info.length_ : pos >= info.length_ ? info.length_ : pos;
        info.bt_op = 1;
        info.bt_pos = p;
        info.bt_len = 0;
        stack.push_undo();
        info.piece.insert(p, str);
        info.updateLength();
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::replace(size_t pos, const T * str) {
        replace(std::string("replace position ") + std::to_string(pos) + " (with a length of 1) with " + str, pos, str);
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::replace(const std::string & undo_tag, size_t pos, const T * str) {
        replace(undo_tag, pos, 1, str);
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::replace(size_t pos, size_t len, const T * str) {
        replace(std::string("replace position ") + std::to_string(pos) + " (with a length of " + std::to_string(len) + ") with " + str, pos, len, str);
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::replace(const std::string & undo_tag, size_t pos, size_t len, const T * str) {
        info.tag = undo_tag;
        
        // stay inside bounds 0 to length_
        size_t p = pos == -1 ? info.length_ : pos >= info.length_ ? info.length_ : pos;
        
        size_t l = 0;
        
        if (p != info.length_) {
            // stay inside bounds 0 to length_
            l = len == -1 ? info.length_ : p + len >= info.length_ ? info.length_ : p + len;
        }
        if (l != 0) {
            info.bt_op = 2;
            info.bt_pos = p;
            info.bt_len = l;
        } else {
            info.bt_op = 1;
            info.bt_pos = p;
            info.bt_len = 0;
        }
        stack.push_undo();
        
        
        if (l != 0) {
            info.piece.erase(p, l);
        }
        
        info.piece.insert(p, str);
        info.updateLength();
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::erase(size_t pos, size_t len) {
        erase(std::string("erase position ") + std::to_string(pos) + " with a length of " + std::to_string(len), pos, len);
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::erase(const std::string & undo_tag, size_t pos, size_t len) {
        info.tag = undo_tag;
        
        // stay inside bounds 0 to length_
        size_t p = pos == -1 ? info.length_ : pos >= info.length_ ? info.length_ : pos;
        
        size_t l = 0;
        
        if (p != info.length_) {
            // stay inside bounds 0 to length_
            l = len == -1 ? info.length_ : p + len >= info.length_ ? info.length_ : p + len;
        }
        
        if (l != 0) {
            info.bt_op = 3;
            info.bt_pos = p;
            info.bt_len = l;
            stack.push_undo();
            info.piece.erase(p, l);
            info.updateLength();
        } else {
            // erasing zero length does nothing
            return;
        }
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::Info::updateLineInfo() {
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
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::Info::updateLength() {
        updateLineInfo();
        length_ = piece.length_cached();
        if (length_ > 0) {
            length_--;
        }
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::seek(size_t pos) {
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
    
    template <typename T, T new_line>
    bool MiniDoc<T, new_line>::has_next() const {
        return info.cursor_ != info.length_;
    }
    
    template <typename T, T new_line>
    T MiniDoc<T, new_line>::next() {
        T ch = character();
        if (has_next()) {
            info.cursor_++;
            info.updateLineInfo();
        }
        return ch;
    }
    
    template <typename T, T new_line>
    bool MiniDoc<T, new_line>::has_previous() const {
        return info.cursor_ != 0;
    }
    
    template <typename T, T new_line>
    T MiniDoc<T, new_line>::previous() {
        T ch = character();
        if (has_previous()) {
            info.cursor_--;
            info.updateLineInfo();
        }
        return ch;
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::Info::print(std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        print("", conv);
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::Info::print(const char * indent, std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        const T * i = indent == nullptr ? "" : indent;
        printf("%stag: %s\n", i, tag.c_str());
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
        const char * r;
        char tmp[2];
        if (c == '\n') {
            r = "<newline>";
        } else if (c == '\t') {
            r = "<tab>";
        } else if (c == '\r') {
            r = "<carrage return>";
        } else if (c == '\0') {
            r = "<null>";
        } else {
            tmp[0] = c;
            tmp[1] = '\0';
            r = tmp;
        }
        
        printf("%scharacter:  '%s'\n", i, r);
    }
    
    template <typename T, T new_line>
    T MiniDoc<T, new_line>::Info::character() const {
        return character_;
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::Info::cursor() const {
        return cursor_;
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::Info::line_start() const {
        return line_start_;
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::Info::line_end() const {
        return line_end_;
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::Info::line_length() const {
        return line_length_;
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::Info::line() const {
        return line_;
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::Info::column() const {
        return column_;
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::Info::length() const {
        return length_;
    }
    template <typename T, T new_line>
    uint8_t MiniDoc<T, new_line>::Info::btop() const {
        return bt_op;
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::Info::btpos() const {
        return bt_pos;
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::Info::btlen() const {
        return bt_len;
    }
    template <typename T, T new_line>
    T MiniDoc<T, new_line>::character() const {
        return info.character();
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::cursor() const {
        return info.cursor();
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::line_start() const {
        return info.line_start();
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::line_end() const {
        return info.line_end();
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::line_length() const {
        return info.line_length();
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::line() const {
        return info.line();
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::column() const {
        return info.column();
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::length() const {
        return info.length();
    }
    template <typename T, T new_line>
    uint8_t MiniDoc<T, new_line>::btop() const {
        return info.btop();
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::btpos() const {
        return info.btpos();
    }
    template <typename T, T new_line>
    size_t MiniDoc<T, new_line>::btlen() const {
        return info.btlen();
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::Info::line_str(std::basic_string<T> & out) const {
        piece.range_string(line_start_, line_end_, out);
    }
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::Info::line_str(std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        piece.range_string_func(line_start_, line_end_, out, func);
    }
    template <typename T, T new_line>
    std::basic_string<T> MiniDoc<T, new_line>::Info::line_str() const {
        std::basic_string<T> s;
        line_str(s);
        return s;
    }
    template <typename T, T new_line>
    std::basic_string<T> MiniDoc<T, new_line>::Info::line_str(std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        std::basic_string<T> s;
        line_str(s, func);
        return s;
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::line_str(std::basic_string<T> & out) const {
        info.line_str(out);
    }
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::line_str(std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        info.line_str(out, func);
    }
    template <typename T, T new_line>
    std::basic_string<T> MiniDoc<T, new_line>::line_str() const {
        std::basic_string<T> s;
        line_str(s);
        return s;
    }
    template <typename T, T new_line>
    std::basic_string<T> MiniDoc<T, new_line>::line_str(std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        std::basic_string<T> s;
        line_str(s, func);
        return s;
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::Info::str(std::basic_string<T> & out) const {
        piece.range_string(0, length_, out);
    }
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::Info::str(std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        piece.range_string_func(0, length_, out, func);
    }
    template <typename T, T new_line>
    std::basic_string<T> MiniDoc<T, new_line>::Info::str() const {
        std::basic_string<T> s;
        str(s);
        return s;
    }
    template <typename T, T new_line>
    std::basic_string<T> MiniDoc<T, new_line>::Info::str(std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        std::basic_string<T> s;
        str(s, func);
        return s;
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::str(std::basic_string<T> & out) const {
        info.str(out);
    }
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::str(std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        info.str(out, func);
    }
    template <typename T, T new_line>
    std::basic_string<T> MiniDoc<T, new_line>::str() const {
        std::basic_string<T> s;
        str(s);
        return s;
    }
    template <typename T, T new_line>
    std::basic_string<T> MiniDoc<T, new_line>::str(std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        std::basic_string<T> s;
        str(s, func);
        return s;
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::Info::sub_str(size_t pos, size_t len, std::basic_string<T> & out) const {
        auto p = pos == -1 ? length_ : pos >= length_ ? length_ : pos;
        piece.range_string(p, p+len, out);
    }
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::Info::sub_str(size_t pos, size_t len, std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        auto p = pos == -1 ? length_ : pos >= length_ ? length_ : pos;
        piece.range_string_func(p, p+len, out, func);
    }
    template <typename T, T new_line>
    std::basic_string<T> MiniDoc<T, new_line>::Info::sub_str(size_t pos, size_t len) const {
        std::basic_string<T> s;
        sub_str(pos, len, s);
        return s;
    }
    template <typename T, T new_line>
    std::basic_string<T> MiniDoc<T, new_line>::Info::sub_str(size_t pos, size_t len, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        std::basic_string<T> s;
        sub_str(pos, len, s, func);
        return s;
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::sub_str(size_t pos, size_t len, std::basic_string<T> & out) const {
        info.sub_str(pos, len, out);
    }
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::sub_str(size_t pos, size_t len, std::basic_string<T> & out, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        info.sub_str(pos, len, out, func);
    }
    template <typename T, T new_line>
    std::basic_string<T> MiniDoc<T, new_line>::sub_str(size_t pos, size_t len) const {
        std::basic_string<T> s;
        sub_str(pos, len, s);
        
        return s;
    }
    template <typename T, T new_line>
    std::basic_string<T> MiniDoc<T, new_line>::sub_str(size_t pos, size_t len, std::function<void(std::basic_string<T> & out, const T *string, size_t length)> func) const {
        std::basic_string<T> s;
        sub_str(pos, len, s, func);
        return s;
    }
    
    template <typename T, T new_line>
    UndoStackHolder<typename MiniDoc<T, new_line>::MiniDoc::Info> MiniDoc<T, new_line>::undoStack() {
        return stack;
    }
    
    template <typename T, T new_line>
    void MiniDoc<T, new_line>::print(std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        info.print(conv);
        stack.print([&](const Info & i) { i.print("    ", conv); });
        printf("\n");
    }
}
#endif