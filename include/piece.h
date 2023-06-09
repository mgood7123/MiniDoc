//
// Created by Alex on 2020/5/7.
//

#ifndef MINIDOC_PIECE_TABLE_H
#define MINIDOC_PIECE_TABLE_H

#include <string>
#include <functional>
#include <vector>
#include <set>
#include <iostream>

#include "cache.h"
#include <string_adapter.h>

namespace MiniDoc {
    
    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    class PieceTable {
        private:
        using buffer_idx_t = uint16_t;
        using iter_func = std::function<void(const char_t *string, size_t length)>;
        enum {
            Append,
            Insert
        };
        public:
        class Piece;
        using offset_t = uint32_t;
        using iter_t = typename std::set<Piece>::iterator;
        
        PieceTable() {
            //puts("PieceTable ctor");
            m_buffers.resize(2);
        }
        
        struct Buffer {
            char_t empty[0];
            const char_t *map_ptr = nullptr;
            // Append-Only Buffer
            std::shared_ptr<adapter_t> buffer;
            // Line index in buffer
            std::vector<uint32_t> lines;
            
            Buffer() {
                buffer = std::make_shared<adapter_t>(empty);
            }
            
            Buffer(const char_t *ptr, size_t length) {
                set_map(ptr, length);
            }
            
            inline void set_map(const char_t *ptr, size_t length) {
                map_ptr = ptr;
                char_t new_line = adapter_t().get_new_line();
                for (size_t index = 0; index < length; ++index) {
                    if (ptr[index] == new_line) {
                        lines.push_back(index);
                    }
                }
            }
            
            inline size_t size() const { return buffer->size(); }
            
            inline void append(const adapter_t &string) {
                size_t offset = size();
                buffer->append_(string);
                char_t new_line = string.get_new_line();
                for (auto ch : string) {
                    if (ch == new_line) {
                        lines.push_back(offset);
                    }
                    offset++;
                }
            }
            
            inline const char_t &operator[](const size_t &index) const {
                if (map_ptr) {
                    return *(map_ptr + index);
                }
                return buffer->operator[](index);
            }
        };
        struct Piece {
            mutable buffer_idx_t buffer = Append;
            mutable uint32_t buffer_lines = 0;
            mutable uint32_t buffer_line_offset = 0;
            mutable uint32_t start = 0;
            mutable uint32_t length = 0;
            mutable uint32_t left_lines = 0;
            mutable uint32_t left_length = 0;
            
            Piece() = default;
            
            Piece(uint32_t left_length, uint32_t left_lines) : left_length(left_length), left_lines(left_lines) {}
            
            inline bool operator<(const Piece &rhs) const {
                if (rhs.left_length == ~0) {
                    return left_lines < rhs.left_lines;
                }
                return left_length < rhs.left_length;
            }
            
            void dump() const {
                if (buffer == 0) {
                    std::cout << "append ";
                }
                if (buffer == 1) {
                    std::cout << "insert ";
                }
                if (buffer > 1) {
                    std::cout << "origin ";
                }
                std::cout << "  left_length: "
                << left_length << "  length:" << length
                << "  left_lines:" << left_lines << "  lines:" << buffer_lines
                << "  line_offset:" << buffer_line_offset;
            }
        };
        class Iterator {
            private:
            PieceTable *m_piece = nullptr;
            offset_t m_start;
            iter_t iter_start;
            iter_t iter_end;
            iter_t iter;
            offset_t m_remainder = 0;
            const char_t *m_string = nullptr;
            size_t m_length = 0;
            public:
            
            Iterator() = default;
            
            Iterator(PieceTable *piece, offset_t start, offset_t end) : m_piece(piece) {
                m_start = start;
                m_remainder = end - start;
                iter = iter_start = m_piece->upper_pos(start);
                iter_end = m_piece->upper_pos(end);
            }
            
            inline bool empty() const { return !m_piece; }
            
            inline const char_t *c_str() const { return m_string; }
            
            inline size_t length() const { return m_length; }
            
            inline adapter_t string() const { return adapter_t(m_string, m_length); }
            
            inline bool next() {
                if (m_remainder == 0) {
                    return false;
                }
                if (iter_start == iter_end) {
                    offset_t offset = m_start - iter_start->left_length;
                    m_string = &m_piece->m_buffers[iter_start->buffer][iter_start->start + offset];
                    m_length = m_remainder;
                    m_remainder = 0;
                    return true;
                }
                if (iter == iter_start) {
                    offset_t offset = m_start - iter->left_length;
                    m_string = &m_piece->m_buffers[iter->buffer][iter->start + offset];
                    m_length = iter->length - offset;
                } else if (iter == iter_end) {
                    m_string = &m_piece->m_buffers[iter->buffer][iter->start];
                    m_length = m_remainder;
                } else {
                    m_string = &m_piece->m_buffers[iter->buffer][iter->start];
                    m_length = iter->length;
                }
                iter++;
                if (m_remainder >= m_length) {
                    m_remainder -= m_length;
                } else {
                    m_remainder = 0;
                }
                return true;
            }
        };
        
        size_t size() const {
            return length();
        }
        
        size_t size_cached() const {
            return length_cached();
        }
        
        size_t length() const {
            if (m_pieces.empty()) {
                return 0;
            }
            auto iter = --m_pieces.end();
            return iter->left_length + iter->length;
        }
        
        size_t length_cached() const {
            return cache_length(this);
        }
        
        size_t lines() const {
            if (m_pieces.empty()) {
                return 0;
            }
            auto iter = --m_pieces.end();
            return iter->left_lines + iter->buffer_lines;
        }
        
        size_t lines_cached() const {
            return cache_lines(this);
        }
        
        // it does not make sense to cache this
        size_t get_line(offset_t pos) const {
            auto iter = upper_pos(pos);
            offset_t offset = pos - iter->left_length; // length in the piece
            offset_t buffer_offset = iter->start + offset; // offset in the buffer
            auto &buffer = m_buffers[iter->buffer];
            auto iter_start = buffer.lines.begin() + iter->buffer_line_offset;
            auto iter_end = iter_start + iter->buffer_lines;
            auto find = std::lower_bound(iter_start, iter_end, buffer_offset);
            auto l = iter->left_lines + (find - iter_start);
            /*
            if (l == 0) {
                return l;
            }
            if (pos <= line_end_cached(l)) {
                return l-1;
            }
            */
            return l;
        }
        
        size_t line_length(size_t line) const {
            return line_end_cached(line) - line_start_cached(line);
        }
        
        offset_t line_start(size_t line) const {
            if (line == 0) {
                return 0;
            }
            return line_end_cached(line - 1) + 1;
        }
        
        offset_t line_start_cached(size_t line) const {
            return cache_line_start(this, line);
        }
        
        offset_t line_end(size_t line) const {
            if (line >= lines_cached()) {
                return size_cached();
            }
            auto iter = lower_line(line); // cached
            size_t offset = line - iter->left_lines;
            auto &buffer = m_buffers[iter->buffer];
            auto start = buffer.lines[iter->buffer_line_offset + offset];
            return iter->left_length + start - iter->start;
        }
        
        offset_t line_end_cached(size_t line) const {
            return cache_line_end(this, line);
        }
        
        iter_t append(const adapter_t &string) {
            Piece piece = feed(string, Append);
            piece.left_lines = lines_cached();
            piece.left_length = size_cached();
            auto r = std::get<0>(m_pieces.emplace(piece));
            caches.invalidate();
            return r;
        }
        
        iter_t insert(offset_t pos, const adapter_t &string) {
            if (pos >= size()) {
                return append(string);
            }
            auto iter = split(pos);
            Piece pt = feed(string, Insert);
            pt.left_lines = iter->left_lines;
            pt.left_length = iter->left_length;
            fixup(iter, pt.length, pt.buffer_lines);
            auto r = m_pieces.insert(iter, pt);
            caches.invalidate();
            return r;
        }
        
        uint32_t erase(offset_t start, offset_t end) {
            auto iter_start = split(start);
            auto iter_end = split(end);
            int32_t delta_length = 0;
            int32_t delta_lines = 0;
            for (auto iter = iter_start; iter != iter_end; iter++) {
                delta_length += iter->length;
                delta_lines += iter->buffer_lines;
            }
            m_pieces.erase(iter_start, iter_end);
            fixup(iter_end, -delta_length, -delta_lines);
            caches.invalidate();
            return delta_lines;
        }
        
        // it does not make sense to cache this
        inline const char_t &char_at(offset_t pos) const {
            auto node = upper_pos(pos);
            size_t offset = pos - node->left_length; // length in the piece
            return m_buffers[node->buffer][node->start + offset];
        }
        
        inline const char_t &operator[](const size_t &index) const { return char_at(index); }
        
        Iterator iter(offset_t start, offset_t end) const {
            return Iterator(this, start, end);
        }
        
        void iter_range(offset_t start, offset_t end, iter_func func) const {
            auto iter_start = upper_pos(start);
            auto iter_end = upper_pos(end);
            offset_t offset = start - iter_start->left_length;
            if (iter_start == iter_end) {
                func(&m_buffers[iter_start->buffer][iter_start->start + offset], end - start);
                return;
            }
            offset_t length = end - (iter_start->left_length + iter_start->length);
            func(&m_buffers[iter_start->buffer][iter_start->start + offset],
            iter_start->length - offset);
            while (length > 0) {
                iter_start++;
                if (length > iter_start->length) {
                    func(&m_buffers[iter_start->buffer][iter_start->start], iter_start->length);
                    length -= iter_start->length;
                } else {
                    func(&m_buffers[iter_start->buffer][iter_start->start], length);
                    length = 0;
                }
                if (iter_start == iter_end) {
                    break;
                }
            }
        }
        
        // caching this would be wasteful
        adapter_t line_string(size_t line) const {
            adapter_t string;
            line_string(line, string);
            return string;
        }
        
        // caching this would be wasteful
        void line_string(size_t line, adapter_t & out) const {
            range_string(line_start(line), line_end(line), out);
        }
        
        // caching this would be wasteful
        adapter_t range_string(offset_t start, offset_t end) const {
            adapter_t string;
            range_string(start, end, string);
            return string;
        }
        
        // caching this would be wasteful
        void range_string(offset_t start, offset_t end, adapter_t & out) const {
            if (end - start == 0) {
                out.resize(0);
                return;
            }
            out.resize(end - start + 1);
            
            iter_range(start, end, [&](const char_t *str, size_t length) {
                out.append(str, length);
            });
        }
        
        // caching this would be wasteful
        adapter_t line_string_func(size_t line, std::function<void(std::string & out, const char_t *string, size_t length)> func) const {
            adapter_t string;
            line_string_func(line, string, func);
            return string;
        }
        
        // caching this would be wasteful
        void line_string_func(size_t line, adapter_t & out, std::function<void(std::string & out, const char_t *string, size_t length)> func) const {
            range_string_func(line_start(line), line_end(line), out, func);
        }
        
        // caching this would be wasteful
        adapter_t range_string_func(offset_t start, offset_t end, std::function<void(std::string & out, const char_t *string, size_t length)> func) const {
            adapter_t string;
            range_string_func(start, end, string, func);
            return string;
        }
        
        // caching this would be wasteful
        void range_string_func(offset_t start, offset_t end, adapter_t & out, std::function<void(std::string & out, const char_t *string, size_t length)> func) const {
            if (end - start == 0) {
                out.resize(0);
                return;
            }
            out.resize(end - start + 1);
            
            iter_range(start, end, [&](const char_t *str, size_t length) {
                func(out, str, length);
            });
        }
        
        iter_t append_origin(const char_t *map, size_t length) {
            Piece piece;
            piece.buffer = m_buffers.size();
            piece.start = 0;
            piece.length = length;
            piece.left_lines = lines_cached();
            piece.left_length = size_cached();
            
            m_buffers.emplace_back(map, length);
            auto &back = m_buffers.back();
            
            piece.buffer_lines = back.lines.size();
            auto r = std::get<0>(m_pieces.emplace(piece));
            caches.invalidate();
            return r;
        }
        
        iter_t insert_origin(offset_t pos, const char_t *map, size_t length) {
            auto iter = split(pos);
            Piece pt;
            pt.buffer = m_buffers.size();
            pt.start = 0;
            pt.length = length;
            
            m_buffers.emplace_back(map, length);
            auto &back = m_buffers.back();
            pt.buffer_lines = back.lines.size();
            
            pt.left_length = iter->left_length;
            pt.left_lines = iter->left_lines;
            fixup(iter, pt.length, pt.buffer_lines);
            auto r = m_pieces.insert(iter, pt);
            caches.invalidate();
            return r;
        }
        
        void dump() const {
            dump(false, false);
        }
        void dumpAll() const {
            dump(true, true);
        }
        void dump(bool print_line, bool print_node_string) const {
            if (print_line) {
                for (int i = 0; i < lines_cached(); ++i) {
                    auto s = line_string(i);
                    std::cout << "[" << line_start(i) << ", " << line_end(i) << "] "
                    << i << ": " << s.c_str() << std::endl;
                }
            }
            for (auto &ittt : m_pieces) {
                ittt.dump();
                if (print_node_string) {
                    std::string string(&m_buffers[ittt.buffer][ittt.start], ittt.length);
                    std::cout << " value:" << string;
                }
                std::cout << std::endl;
            }
        }
        
        private:
        
        iter_t split(offset_t pos) {
            if (pos >= size_cached()) {
                return m_pieces.end();
            }
            auto iter = upper_pos(pos);
            if (iter->left_length == pos) {
                return iter;
            }
            if (iter->left_length + iter->length == pos) {
                return --iter;
            }
            Piece pt = *iter;
            iter->length = pos - iter->left_length;
            calc_line(*iter);
            
            pt.start = pt.start + iter->length;
            pt.length = pt.length - iter->length;
            pt.left_lines = iter->left_lines + iter->buffer_lines;
            pt.left_length = iter->left_length + iter->length;
            pt.buffer_lines -= iter->buffer_lines;
            pt.buffer_line_offset += iter->buffer_lines;
            //calc_line(pt);
            auto r = m_pieces.insert(iter, pt);
            caches.invalidate();
            return r;
        }
        
        // it does not make sense to cache this
        inline iter_t upper_pos(offset_t pos) const {
            return --m_pieces.upper_bound(Piece(pos, ~0));
        }
        
        // called by size_t line_end(line)
        inline iter_t lower_line(size_t line) const {
            return find_line_cached(line + 1);
        }
        
        inline iter_t find_line(size_t line) const {
            auto iter = m_pieces.lower_bound(Piece(~0, line));
            if (iter == m_pieces.end()) {
                --iter;
            }
            //!(iter->left_lines < line && line <= iter->left_lines + iter->buffer_lines)
            while (iter->left_lines >= line || iter->buffer_lines == 0) {
                --iter;
            }
            return iter;
        }
        
        inline iter_t find_line_cached(size_t line) const {
            return cache_find_line(this, line);
        }
        
        // this cannot be cached
        inline void calc_line(const Piece &piece) const {
            offset_t buffer_offset = piece.start + piece.length;
            auto iter_begin = m_buffers[piece.buffer].lines.begin() + piece.buffer_line_offset;
            auto iter_end = iter_begin + piece.buffer_lines;
            // can maybe cache this?
            size_t buffer_lines = std::lower_bound(iter_begin, iter_end, buffer_offset) - iter_begin;
            piece.buffer_lines = buffer_lines;
            /*auto iter = &(m_buffers[piece.buffer][piece.start]);
            auto end = &(m_buffers[piece.buffer][piece.start + piece.length]);
            piece.buffer_lines = 0;
            for (; iter != end; iter++) {
            if (*iter == new_line) {
            piece.buffer_lines++;
            }
            }*/
        }
        
        // this gets called before invalidating the caches
        inline void fixup(iter_t iter, int32_t delta_length = 0, int32_t delta_lines = 0) const {
            for (auto end = m_pieces.end(); iter != end; iter++) {
                iter->left_length += delta_length;
                iter->left_lines += delta_lines;
            }
        }
        
        inline Piece feed(const adapter_t &string, buffer_idx_t index) {
            Piece piece;
            piece.buffer = index;
            piece.start = m_buffers[piece.buffer].size();
            piece.length = string.length();
            piece.buffer_line_offset = m_buffers[piece.buffer].lines.size();
            m_buffers[piece.buffer].append(string);
            piece.buffer_lines = m_buffers[piece.buffer].lines.size() - piece.buffer_line_offset;
            caches.invalidate();
            return piece;
        }
        
        decltype(MiniDoc::CacheHelper::Get("PieceTable<char_t, adapter_t>::line_start", &PieceTable<char_t, adapter_t>::line_start)) cache_line_start = MiniDoc::CacheHelper::Get("PieceTable<char_t, adapter_t>::line_start", &PieceTable<char_t, adapter_t>::line_start);
        
        decltype(MiniDoc::CacheHelper::Get("PieceTable<char_t, adapter_t>::line_end", &PieceTable<char_t, adapter_t>::line_end)) cache_line_end = MiniDoc::CacheHelper::Get("PieceTable<char_t, adapter_t>::line_end", &PieceTable<char_t, adapter_t>::line_end);
        
        decltype(MiniDoc::CacheHelper::Get("PieceTable<char_t, adapter_t>::length", &PieceTable<char_t, adapter_t>::length)) cache_length = MiniDoc::CacheHelper::Get("PieceTable<char_t, adapter_t>::length", &PieceTable<char_t, adapter_t>::length);
        
        decltype(MiniDoc::CacheHelper::Get("PieceTable<char_t, adapter_t>::lines", &PieceTable<char_t, adapter_t>::lines)) cache_lines = MiniDoc::CacheHelper::Get("PieceTable<char_t, adapter_t>::lines", &PieceTable<char_t, adapter_t>::lines);
        
        decltype(MiniDoc::CacheHelper::Get("PieceTable<char_t, adapter_t>::find_line", &PieceTable<char_t, adapter_t>::find_line)) cache_find_line = MiniDoc::CacheHelper::Get("PieceTable<char_t, adapter_t>::find_line", &PieceTable<char_t, adapter_t>::find_line);
        
        CacheInvalidator caches {&cache_line_start, &cache_line_end, &cache_length, &cache_lines, &cache_find_line};
        
        private:
        
        std::vector<Buffer> m_buffers;
        std::set<Piece> m_pieces;
    };

    using PieceTable_T = PieceTable<char, StringAdapter::CharAdapter>;
}
#endif
