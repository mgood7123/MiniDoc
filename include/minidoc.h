#ifndef MINIDOC_H
#define MINIDOC_H

#include <cstddef> // std::nullopt_t

#include "cache.h"
#include "piece.h"
#include "undo.h"

#define MINIDOC_TEMPLATE_DECL \
template <typename T, typename adapter_t>
#define MINIDOC_TEMPLATE_DEF \
MiniDoc<T, adapter_t>
#define MINIDOC_STRING \
adapter_t

namespace MiniDoc {
    MINIDOC_TEMPLATE_DECL
    class MiniDoc {
        T empty[0];
        T new_line = adapter_t().get_new_line();
        T new_line_str[1] { adapter_t().get_new_line() };
        public:
        class Info {
            friend MiniDoc;
            PieceTable<T, adapter_t> piece;
            std::string tag;
            T character_;
            size_t cursor_ = 0;
            size_t line_ = 0;
            size_t lines_ = 0;
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
            size_t lines() const;
            size_t column() const;
            size_t length() const;
            
            std::string bttag() const;
            uint8_t btop() const;
            size_t btpos() const;
            size_t btlen() const;
            
            size_t clamp_pos(size_t pos) {
                size_t length = length_;
                return pos == -1 ? length : pos >= length ? length : pos;
            }
            
            size_t clamp_length(size_t clamped_pos, size_t len) {
                size_t length = length_;
                return clamped_pos == length ? 0 : len == -1 ? length : clamped_pos + len >= length ? length : clamped_pos + len;
            }
            
            void line_str(MINIDOC_STRING & out) const;
            void line_str(MINIDOC_STRING & out, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const;
            MINIDOC_STRING line_str() const;
            MINIDOC_STRING line_str(std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const;
            
            void str(MINIDOC_STRING & out) const;
            void str(MINIDOC_STRING & out, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const;
            MINIDOC_STRING str() const;
            MINIDOC_STRING str(std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const;
            
            void sub_str(size_t pos, size_t len, MINIDOC_STRING & out) const;
            void sub_str(size_t pos, size_t len, MINIDOC_STRING & out, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const;
            MINIDOC_STRING sub_str(size_t pos, size_t len) const;
            MINIDOC_STRING sub_str(size_t pos, size_t len, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const;
            
            void print(std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;
            void print(const char * indent, std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;
            void printDocument(std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;
            void printDocument(const char * indent, std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;
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
        void seek_line(size_t line);
        void seek_line_start();
        void seek_line_start(size_t line);
        void seek_line_end();
        void seek_line_end(size_t line);
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
        size_t lines() const;
        size_t column() const;
        size_t length() const;
        
        std::string bttag() const;
        uint8_t btop() const;
        size_t btpos() const;
        size_t btlen() const;
        
        void line_str(MINIDOC_STRING & out) const;
        void line_str(MINIDOC_STRING & out, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const;
        MINIDOC_STRING line_str() const;
        MINIDOC_STRING line_str(std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const;
        
        void str(MINIDOC_STRING & out) const;
        void str(MINIDOC_STRING & out, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const;
        MINIDOC_STRING str() const;
        MINIDOC_STRING str(std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const;
        
        void sub_str(size_t pos, size_t len, MINIDOC_STRING & out) const;
        void sub_str(size_t pos, size_t len, MINIDOC_STRING & out, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const;
        MINIDOC_STRING sub_str(size_t pos, size_t len) const;
        MINIDOC_STRING sub_str(size_t pos, size_t len, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const;
        
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
        
        void printDocument(std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;
    };
    
    using MiniDoc_T = MiniDoc<char, StringAdapter::CharAdapter>;
}



// source>

#include <stdlib.h> // malloc.h is not portable
#include <cstring> // strlen
#include <hexdump.hpp>

namespace MiniDoc {
    MINIDOC_TEMPLATE_DECL
    MINIDOC_TEMPLATE_DEF::MiniDoc() {
        load(empty);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::load(std::nullptr_t stream) {
        load(empty, 0);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::load(std::nullptr_t stream, size_t length) {
        load(empty, length);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::load(const T* stream) {
        load(stream, strlen(stream));
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::load(const T* stream, size_t length) {
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
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::append(const T * str) {
        append(std::string("append ") + str, str);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::append(const std::string & undo_tag, const T * str) {
        insert(undo_tag, -1, str);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::insert(size_t pos, const T * str) {
        size_t p = info.clamp_pos(pos);
        insert(std::string("insert ") + str + " at position " + std::to_string(p), p, str);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::insert(const std::string & undo_tag, size_t pos, const T * str) {
        
        stack.push_undo();
        
        info.tag = undo_tag;
        
        // stay inside bounds 0 to length_
        size_t p = info.clamp_pos(pos);
        info.bt_op = 1;
        info.bt_pos = p;
        info.bt_len = 0;
        info.piece.insert(p, str);
        info.updateLength();
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::replace(size_t pos, const T * str) {
        size_t p = info.clamp_pos(pos);
        replace(std::string("replace position ") + std::to_string(p) + " (with a length of 1) with " + str, p, str);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::replace(const std::string & undo_tag, size_t pos, const T * str) {
        replace(undo_tag, pos, 1, str);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::replace(size_t pos, size_t len, const T * str) {
        size_t p = info.clamp_pos(pos);
        size_t l = info.clamp_length(p, len);
        replace(std::string("replace position ") + std::to_string(p) + " (with a length of " + std::to_string(l) + ") with " + str, p, l, str);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::replace(const std::string & undo_tag, size_t pos, size_t len, const T * str) {
        
        stack.push_undo();
        
        info.tag = undo_tag;
        
        // stay inside bounds 0 to length_
        size_t p = info.clamp_pos(pos);
        if (p >= length()) {
            info.bt_op = 1;
            info.bt_pos = p;
            info.bt_len = 0;
            info.piece.insert(p, str);
            info.updateLength();
        } else {
            size_t l = info.clamp_length(p, len);
            
            if (l != 0) {
                info.bt_op = 2;
                info.bt_pos = p;
                info.bt_len = l-p;
                info.piece.erase(p, l);
            } else {
                info.bt_op = 1;
                info.bt_pos = p;
                info.bt_len = 0;
            }
            
            info.piece.insert(p, str);
            info.updateLength();
        }
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::erase(size_t pos, size_t len) {
        size_t p = info.clamp_pos(pos);
        size_t l = info.clamp_length(p, len);
        erase(std::string("erase position ") + std::to_string(p) + " with a length of " + std::to_string(l), p, l);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::erase(const std::string & undo_tag, size_t pos, size_t len) {
        
        // stay inside bounds 0 to length_
        size_t p = info.clamp_pos(pos);
        if (p >= length()) {
            // erasing end does nothing
            return;
        }
        
        size_t l = info.clamp_length(p, len);
        if (l != 0) {
            stack.push_undo();
            info.tag = undo_tag;
            info.bt_op = 3;
            info.bt_pos = p;
            info.bt_len = l-p;
            info.piece.erase(p, l);
            info.updateLength();
        } else {
            // erasing zero length does nothing
            return;
        }
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::Info::updateLineInfo() {
        line_ = piece.get_line(cursor_);
        lines_ = piece.lines_cached();
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
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::Info::updateLength() {
        updateLineInfo();
        length_ = piece.length_cached();
        if (length_ > 0) {
            length_--;
        }
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::seek(size_t pos) {
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
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::seek_line(size_t line) {
        if (line == info.line_) {
            return;
        }
        auto l = info.lines_-1;
        if (line == -1) {
            line = l;
        }
        if (line > l) {
            line = l;
        }
        //info.piece.dumpAll();
        if (line > info.line_) {
            while (line != info.line_) {
                next();
            }
        } else {
            while (line != info.line_) {
                previous();
            }
        }
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::seek_line_start() {
        seek(info.line_start_);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::seek_line_start(size_t line) {
        seek_line(line);
        seek_line_start();
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::seek_line_end() {
        seek(info.line_end_);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::seek_line_end(size_t line) {
        seek_line(line);
        seek_line_end();
    }
    
    MINIDOC_TEMPLATE_DECL
    bool MINIDOC_TEMPLATE_DEF::has_next() const {
        return info.cursor_ != info.length_;
    }
    
    MINIDOC_TEMPLATE_DECL
    T MINIDOC_TEMPLATE_DEF::next() {
        T ch = character();
        if (has_next()) {
            info.cursor_++;
            info.updateLineInfo();
        }
        return ch;
    }
    
    MINIDOC_TEMPLATE_DECL
    bool MINIDOC_TEMPLATE_DEF::has_previous() const {
        return info.cursor_ != 0;
    }
    
    MINIDOC_TEMPLATE_DECL
    T MINIDOC_TEMPLATE_DEF::previous() {
        T ch = character();
        if (has_previous()) {
            info.cursor_--;
            info.updateLineInfo();
            /*
            if (info.character_ == new_line) {
                if (info.line_ != 0) info.line_--;
            }
            */
        }
        return ch;
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::Info::print(std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        print("", conv);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::Info::print(const char * indent, std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        const T * i = indent == nullptr ? "" : indent;
        printf("%stag: %s\n", i, tag.c_str());
        printf("%slines: %zu\n", i, lines_);
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
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::Info::printDocument(std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        printDocument("", conv);
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::Info::printDocument(const char * indent, std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        const T * i = indent == nullptr ? "" : indent;
        printf("%slines: %zu\n", i, lines_);
        printf("%slength: %zu\n", i, length_);
        auto s = str();
        printf("%sdocument: %s\n", i, s.c_str());
        printf("%sdocument size: %zu\n", i, s.size());
        printf("%sdocument len: %zu\n", i, s.length());
        printf("%sdocument hex\n", i);
        std::cout << CustomHexdump<8, true, T>("        ", s.c_str(), s.size(), conv) << std::endl;
    }
    
    MINIDOC_TEMPLATE_DECL
    T MINIDOC_TEMPLATE_DEF::Info::character() const {
        return character_;
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::Info::cursor() const {
        return cursor_;
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::Info::line_start() const {
        return line_start_;
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::Info::line_end() const {
        return line_end_;
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::Info::line_length() const {
        return line_length_;
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::Info::line() const {
        return line_;
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::Info::lines() const {
        return lines_;
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::Info::column() const {
        return column_;
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::Info::length() const {
        return length_;
    }
    MINIDOC_TEMPLATE_DECL
    std::string MINIDOC_TEMPLATE_DEF::Info::bttag() const {
        return tag;
    }
    MINIDOC_TEMPLATE_DECL
    uint8_t MINIDOC_TEMPLATE_DEF::Info::btop() const {
        return bt_op;
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::Info::btpos() const {
        return bt_pos;
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::Info::btlen() const {
        return bt_len;
    }
    MINIDOC_TEMPLATE_DECL
    T MINIDOC_TEMPLATE_DEF::character() const {
        return info.character();
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::cursor() const {
        return info.cursor();
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::line_start() const {
        return info.line_start();
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::line_end() const {
        return info.line_end();
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::line_length() const {
        return info.line_length();
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::line() const {
        return info.line();
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::lines() const {
        return info.lines();
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::column() const {
        return info.column();
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::length() const {
        return info.length();
    }
    MINIDOC_TEMPLATE_DECL
    std::string MINIDOC_TEMPLATE_DEF::bttag() const {
        return info.bttag();
    }
    MINIDOC_TEMPLATE_DECL
    uint8_t MINIDOC_TEMPLATE_DEF::btop() const {
        return info.btop();
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::btpos() const {
        return info.btpos();
    }
    MINIDOC_TEMPLATE_DECL
    size_t MINIDOC_TEMPLATE_DEF::btlen() const {
        return info.btlen();
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::Info::line_str(MINIDOC_STRING & out) const {
        piece.range_string(line_start_, line_end_, out);
    }
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::Info::line_str(MINIDOC_STRING & out, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const {
        piece.range_string_func(line_start_, line_end_, out, func);
    }
    MINIDOC_TEMPLATE_DECL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::Info::line_str() const {
        MINIDOC_STRING s;
        line_str(s);
        return s;
    }
    MINIDOC_TEMPLATE_DECL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::Info::line_str(std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const {
        MINIDOC_STRING s;
        line_str(s, func);
        return s;
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::line_str(MINIDOC_STRING & out) const {
        info.line_str(out);
    }
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::line_str(MINIDOC_STRING & out, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const {
        info.line_str(out, func);
    }
    MINIDOC_TEMPLATE_DECL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::line_str() const {
        MINIDOC_STRING s;
        line_str(s);
        return s;
    }
    MINIDOC_TEMPLATE_DECL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::line_str(std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const {
        MINIDOC_STRING s;
        line_str(s, func);
        return s;
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::Info::str(MINIDOC_STRING & out) const {
        piece.range_string(0, length_, out);
    }
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::Info::str(MINIDOC_STRING & out, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const {
        piece.range_string_func(0, length_, out, func);
    }
    MINIDOC_TEMPLATE_DECL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::Info::str() const {
        MINIDOC_STRING s;
        str(s);
        return s;
    }
    MINIDOC_TEMPLATE_DECL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::Info::str(std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const {
        MINIDOC_STRING s;
        str(s, func);
        return s;
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::str(MINIDOC_STRING & out) const {
        info.str(out);
    }
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::str(MINIDOC_STRING & out, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const {
        info.str(out, func);
    }
    MINIDOC_TEMPLATE_DECL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::str() const {
        MINIDOC_STRING s;
        str(s);
        return s;
    }
    MINIDOC_TEMPLATE_DECL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::str(std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const {
        MINIDOC_STRING s;
        str(s, func);
        return s;
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::Info::sub_str(size_t pos, size_t len, MINIDOC_STRING & out) const {
        auto p = pos == -1 ? length_ : pos >= length_ ? length_ : pos;
        piece.range_string(p, p+len, out);
    }
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::Info::sub_str(size_t pos, size_t len, MINIDOC_STRING & out, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const {
        auto p = pos == -1 ? length_ : pos >= length_ ? length_ : pos;
        piece.range_string_func(p, p+len, out, func);
    }
    MINIDOC_TEMPLATE_DECL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::Info::sub_str(size_t pos, size_t len) const {
        MINIDOC_STRING s;
        sub_str(pos, len, s);
        return s;
    }
    MINIDOC_TEMPLATE_DECL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::Info::sub_str(size_t pos, size_t len, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const {
        MINIDOC_STRING s;
        sub_str(pos, len, s, func);
        return s;
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::sub_str(size_t pos, size_t len, MINIDOC_STRING & out) const {
        info.sub_str(pos, len, out);
    }
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::sub_str(size_t pos, size_t len, MINIDOC_STRING & out, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const {
        info.sub_str(pos, len, out, func);
    }
    MINIDOC_TEMPLATE_DECL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::sub_str(size_t pos, size_t len) const {
        MINIDOC_STRING s;
        sub_str(pos, len, s);
        
        return s;
    }
    MINIDOC_TEMPLATE_DECL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::sub_str(size_t pos, size_t len, std::function<void(MINIDOC_STRING & out, const T *string, size_t length)> func) const {
        MINIDOC_STRING s;
        sub_str(pos, len, s, func);
        return s;
    }
    
    MINIDOC_TEMPLATE_DECL
    UndoStackHolder<typename MINIDOC_TEMPLATE_DEF::MiniDoc::Info> MINIDOC_TEMPLATE_DEF::undoStack() {
        return stack;
    }
    
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::print(std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        info.print(conv);
        stack.print([&](const Info & i) { i.print("    ", conv); });
        printf("\n");
    }
    MINIDOC_TEMPLATE_DECL
    void MINIDOC_TEMPLATE_DEF::printDocument(std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        info.printDocument(conv);
        printf("\n");
    }
}
#endif