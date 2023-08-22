#ifndef MINIDOC_H
#define MINIDOC_H

#include <cstddef> // std::nullopt_t

#include "cache.h"
#include "undo.h"

#include <generic_piece_table.h>
#include <darcs_patch.h>

#define MINIDOC_TEMPLATE_DECL \
    template < \
        typename T, \
        typename adapter_t, \
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<T>, adapter_t>::value>::type \
    >
#define MINIDOC_TEMPLATE_IMPL \
    template < \
        typename T, \
        typename adapter_t, \
        typename AdapterMustExtendBasicStringAdapter \
    >
#define MINIDOC_TEMPLATE_DEF \
    MiniDoc< \
        T, \
        adapter_t, \
        AdapterMustExtendBasicStringAdapter \
    >

#define MINIDOC_STRING adapter_t

namespace MiniDoc {

    #include <hexdump.hpp>

    template <
        typename T,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<T>, adapter_t>::value>::type
    >
    std::string escape(const adapter_t & s) {
        std::string x;
        if (s.size() != 0) {
            for (int i = 0, m = s.size()-1; i <= m; i++) {
                const char c = s[i];
                if (c == '\n') x.append("\\n");
                else if (c == '\t') x.append("\\t");
                else if (c == '\r') x.append("\\r");
                else if (c == '\v') x.append("\\v");
                else if (c == '\b') x.append("\\b");
                else if (c == '\\') x.append("\\\\");
                else x.push_back(c);
            }
        }
        return x;
    }

    std::string escape(const char * s, std::size_t len) {
        std::string x;
        if (len != 0) {
            for (int i = 0, m = len-1; i <= m; i++) {
                const char c = s[i];
                if (c == '\n') x.append("\\n");
                else if (c == '\t') x.append("\\t");
                else if (c == '\r') x.append("\\r");
                else if (c == '\v') x.append("\\v");
                else if (c == '\b') x.append("\\b");
                else if (c == '\\') x.append("\\\\");
                else x.push_back(c);
            }
        }
        return x;
    }

    template <
        typename T,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<T>, adapter_t>::value>::type
    >
    std::size_t escape_index(const adapter_t & unescaped, std::size_t unescaped_index) {
        std::size_t r = 0, n = 0;
        if (unescaped.size() != 0) {
            if (unescaped_index >= unescaped.size()+1) {
                throw std::runtime_error("index out of range");
            }
            for (int i = 0; i <= unescaped_index; i++) {
                r += n;
                const char c = unescaped[i];
                if (c == '\n') n = 2;
                else if (c == '\t') n = 2;
                else if (c == '\r') n = 2;
                else if (c == '\v') n = 2;
                else if (c == '\b') n = 2;
                else if (c == '\\') n = 2;
                else n = 1;
            }
        }
        return r;
    }

    std::size_t escape_index(const char * unescaped, std::size_t len, std::size_t unescaped_index) {
        std::size_t r = 0, n = 0;
        if (len != 0) {
            if (unescaped_index >= len+1) {
                throw std::runtime_error("index out of range");
            }
            for (int i = 0; i <= unescaped_index; i++) {
                r += n;
                const char c = unescaped[i];
                if (c == '\n') n = 2;
                else if (c == '\t') n = 2;
                else if (c == '\r') n = 2;
                else if (c == '\v') n = 2;
                else if (c == '\b') n = 2;
                else if (c == '\\') n = 2;
                else n = 1;
            }
        }
        return r;
    }

    uint8_t numDigits(uint8_t a) {
        if(a < 1) return 0;
        if(a < 10) return 1;
        if(a < 100) return 2;
        return 3;
    }

    uint8_t numDigits(uint16_t a) {
        if(a < 1) return 0;
        if(a < 10) return 1;
        if(a < 100) return 2;
        if(a < 1000) return 3;
        if(a < 10000) return 4;
        return 5;
    }

    uint8_t numDigits(uint32_t a) {
        if(a < 1) return 0;
        if(a < 10) return 1;
        if(a < 100) return 2;
        if(a < 1000) return 3;
        if(a < 10000) return 4;
        if(a < 100000) return 5;
        if(a < 1000000) return 6;
        if(a < 10000000) return 7;
        if(a < 100000000) return 8;
        if(a < 1000000000) return 9;
        return 10;
    }

    uint8_t numDigits(uint64_t a) {
        if(a < 1) return 0;
        if(a < 10) return 1;
        if(a < 100) return 2;
        if(a < 1000) return 3;
        if(a < 10000) return 4;
        if(a < 100000) return 5;
        if(a < 1000000) return 6;
        if(a < 10000000) return 7;
        if(a < 100000000) return 8;
        if(a < 1000000000) return 9;
        if(a < 10000000000) return 10;
        if(a < 100000000000) return 11;
        if(a < 1000000000000) return 12;
        if(a < 10000000000000) return 13;
        if(a < 100000000000000) return 14;
        if(a < 1000000000000000) return 15;
        if(a < 10000000000000000) return 16;
        if(a < 100000000000000000) return 17;
        if(a < 1000000000000000000) return 18;
        if(a < 10000000000000000000ULL) return 19;
        return 20;
    }

    template <
        typename T,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<T>, adapter_t>::value>::type
    >
    struct AdapterPieceTable : public GenericPieceTable<
        std::list<GenericPieceTableDescriptor>,
        std::list<GenericPieceTableDescriptorOrder>,
        const T*, const T*, adapter_t, adapter_t
    > {
        using GPT = GenericPieceTable<std::list<GenericPieceTableDescriptor>, std::list<GenericPieceTableDescriptorOrder>, const T*, const T*, adapter_t, adapter_t>;
        using USER_DATA_USER_DATA_T = typename GPT::USER_DATA_USER_DATA_T;
        using USER_DATA_START_T = typename GPT::USER_DATA_START_T;
        using USER_DATA_ORIGIN_CONTENT_T = typename GPT::USER_DATA_ORIGIN_CONTENT_T;
        using USER_DATA_LENGTH_T = typename GPT::USER_DATA_LENGTH_T;
        using USER_DATA_FLAGS_T = typename GPT::USER_DATA_FLAGS_T;

        using FINSERT_T = MINIDOC_GENERIC_PIECE_TABLE_FUNCTION_TYPE<void(void* instance, const bool & debug, USER_DATA_USER_DATA_T user_data, USER_DATA_START_T start, USER_DATA_ORIGIN_CONTENT_T content, USER_DATA_LENGTH_T length)>;
        using FSPLIT_T = MINIDOC_GENERIC_PIECE_TABLE_FUNCTION_TYPE<void(void* instance, const bool & debug, USER_DATA_USER_DATA_T user_data, USER_DATA_START_T start, USER_DATA_LENGTH_T length, USER_DATA_USER_DATA_T user_data2)>;
        using FERASE_T = MINIDOC_GENERIC_PIECE_TABLE_FUNCTION_TYPE<void(void* instance, const bool & debug, USER_DATA_USER_DATA_T user_data, USER_DATA_START_T start, USER_DATA_LENGTH_T length, USER_DATA_FLAGS_T is_start)>;

        FINSERT_T finsert = [](auto * this_, auto & debug, auto & user_data, auto & start, auto & content, auto & content_length) {};
        FSPLIT_T fsplit = [](auto * this_, auto & debug, auto & user_data, auto & start, auto & length, auto & user_data2) {};
        FERASE_T ferase = [](auto * this_, auto & debug, auto & user_data, auto & start, auto & length, auto & is_start) {};

        AdapterPieceTable(const AdapterPieceTable<T, adapter_t> & other) : GPT(other) {}

        AdapterPieceTable & operator=(const AdapterPieceTable<T, adapter_t> & other) {
            GPT::operator=(other);
            finsert = other.finsert;
            fsplit = other.fsplit;
            ferase = other.ferase;
            return *this;
        }

        AdapterPieceTable() : GPT (
            { // descriptor
                // reset
                [](auto & c) { c = {}; },
                // append
                [](auto & c, auto & d) { c.emplace_back(d); },
                // length
                [](auto & c) { return std::distance(c.begin(), c.end()); },
                // const index
                [](auto & c, auto index) -> const GenericPieceTableDescriptor & { return *std::next(c.begin(), index); },
                // index
                [](auto & c, auto index) -> GenericPieceTableDescriptor & { return *std::next(c.begin(), index); }
            },
            { // descriptor order
                // reset
                [](auto & c) { c = {}; },
                // insert
                [](auto & c, auto & d, auto index) { c.insert(std::next(c.begin(), index), d); },
                // length
                [](auto & c) { return std::distance(c.begin(), c.end()); },
                // const index
                [](auto & c, auto index) -> const GenericPieceTableDescriptorOrder & { return *std::next(c.begin(), index); },
                // index
                [](auto & c, auto index) -> GenericPieceTableDescriptorOrder & { return *std::next(c.begin(), index); }
            },
            { // origin
                // reset
                [](auto & c) { c = adapter_t(); },
                // append
                [](auto & c, auto & content) { c.append(content); },
                // container length
                [](auto & c) { return c.size(); },
                // content length
                [](auto & content) { return content == nullptr ? 0 : adapter_t(content).size(); },
                // container index to char
                [](auto & c, auto index) -> const char { return c.index_to_char(index); },
                // user data insert
                [](auto * this_, auto & debug, auto & user_data, auto & start, auto & content, auto & content_length) { static_cast<AdapterPieceTable<T, adapter_t>*>(this_)->finsert(this_, debug, user_data, start, content, content_length); },
                // user data split
                [](auto * this_, auto & debug, auto & user_data, auto & start, auto & length, auto & user_data_2) { static_cast<AdapterPieceTable<T, adapter_t>*>(this_)->fsplit(this_, debug, user_data, start, length, user_data_2); },
                // user data erase
                [](auto * this_, auto & debug, auto & user_data, auto & start, auto & length, auto & is_start) { static_cast<AdapterPieceTable<T, adapter_t>*>(this_)->ferase(this_, debug, user_data, start, length, is_start); }
            },
            { // append
                // reset
                [](auto & c) { c = adapter_t(); },
                // append
                [](auto & c, auto & content) { c.append(content); },
                // container length
                [](auto & c) { return c.size(); },
                // content length
                [](auto & content) { return content == nullptr ? 0 : adapter_t(content).size(); },
                // content index to char
                [](auto & c, auto index) -> const char { return c.index_to_char(index); },
                // user data insert
                [](auto * this_, auto & debug, auto & user_data, auto & start, auto & content, auto & content_length) { static_cast<AdapterPieceTable<T, adapter_t>*>(this_)->finsert(this_, debug, user_data, start, content, content_length); },
                // user data split
                [](auto * this_, auto & debug, auto & user_data, auto & start, auto & length, auto & user_data_2) { static_cast<AdapterPieceTable<T, adapter_t>*>(this_)->fsplit(this_, debug, user_data, start, length, user_data_2); },
                // user data erase
                [](auto * this_, auto & debug, auto & user_data, auto & start, auto & length, auto & is_start) { static_cast<AdapterPieceTable<T, adapter_t>*>(this_)->ferase(this_, debug, user_data, start, length, is_start); }
            }
        ) {}

        adapter_t range_string_adapter_len(std::size_t start, std::size_t length) const {
            adapter_t adapter;
            range_string_adapter_len(start, length, adapter);
            return adapter;
        }

        void range_string_adapter_len(std::size_t start, std::size_t length, adapter_t & out) const {
            if (length == 0) {
                return;
            }
            auto end = this->length();
            auto position_ = start >= end ? end : start;
            if (position_ == end) {
                return;
            }
            if (length == -1) {
                length = end;
            }
            auto position_end = position_ + length;
            if (position_end >= end) {
                position_end = end;
            }
            auto length_ = position_end - position_;
            if (length_ == 0) {
                return;
            }
            return range_string_adapter(position_, position_end, out);
        }

        adapter_t range_string_adapter(std::size_t start, std::size_t end) const {
            adapter_t adapter;
            range_string_adapter(start, end, adapter);
            return adapter;
        }

        void range_string_adapter(std::size_t start, std::size_t end, adapter_t & out) const {
            auto len = this->length();
            if (end >= len) {
                end = len;
            }
            if (start >= len) {
                start = len;
            }
            if (start >= end) {
                start = end;
            }
            len = end - start;
            if (len == 0) {
                out.resize(0);
                return;
            }

            out.resize(len);

            std::size_t LEN = 0;
            std::size_t COUNT = 0;
            bool found_start = false;
            auto piece_order_size = this->descriptor_count();
            for (size_t i = 0; i < piece_order_size; i++) {
                auto & order = this->descriptor_at(i);
                auto & descriptor = *order.ptr;
                if (descriptor.length == 0) continue;
                auto next_LEN = LEN + descriptor.length;
                if (start < next_LEN) {
                    auto buffer_index = descriptor.start;
                    if (!found_start) {
                        found_start = true;
                        buffer_index += (start - LEN);
                    }
                    auto & info = order.origin ? this->get_origin_info() : this->get_append_info();
                    for (auto i_ = buffer_index; i_ < (descriptor.start + descriptor.length); i_++) {
                        out[COUNT] = info.container_index_to_char(i_);
                        COUNT++;
                        if (COUNT >= len) {
                            return;
                        }
                    }
                } else {
                    LEN = next_LEN;
                }
            }
        }
    };

    template <
        typename T,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<T>, adapter_t>::value>::type
    >
    struct AdapterPieceTableWithLineInfo : public AdapterPieceTable<T, adapter_t> {

        using GPT = AdapterPieceTable<T, adapter_t>;
        using USER_DATA_USER_DATA_T = typename GPT::USER_DATA_USER_DATA_T;
        using USER_DATA_START_T = typename GPT::USER_DATA_START_T;
        using USER_DATA_ORIGIN_CONTENT_T = typename GPT::USER_DATA_ORIGIN_CONTENT_T;
        using USER_DATA_LENGTH_T = typename GPT::USER_DATA_LENGTH_T;
        using USER_DATA_FLAGS_T = typename GPT::USER_DATA_FLAGS_T;

        using FINSERT_T = MINIDOC_GENERIC_PIECE_TABLE_FUNCTION_TYPE<void(void* instance, const bool & debug, USER_DATA_USER_DATA_T user_data, USER_DATA_START_T start, USER_DATA_ORIGIN_CONTENT_T content, USER_DATA_LENGTH_T length)>;
        using FSPLIT_T = MINIDOC_GENERIC_PIECE_TABLE_FUNCTION_TYPE<void(void* instance, const bool & debug, USER_DATA_USER_DATA_T user_data, USER_DATA_START_T start, USER_DATA_LENGTH_T length, USER_DATA_USER_DATA_T user_data2)>;
        using FERASE_T = MINIDOC_GENERIC_PIECE_TABLE_FUNCTION_TYPE<void(void* instance, const bool & debug, USER_DATA_USER_DATA_T user_data, USER_DATA_START_T start, USER_DATA_LENGTH_T length, USER_DATA_FLAGS_T is_start)>;

        private:
        FINSERT_T finsert_ = [](auto * this_, auto & debug, auto & user_data, auto & start, auto & content, auto & content_length) {};
        FSPLIT_T fsplit_ = [](auto * this_, auto & debug, auto & user_data, auto & start, auto & length, auto & user_data2) {};
        FERASE_T ferase_ = [](auto * this_, auto & debug, auto & user_data, auto & start, auto & length, auto & is_start) {};
        public:

        AdapterPieceTableWithLineInfo(const AdapterPieceTableWithLineInfo<T, adapter_t> & other) : GPT(other) {}

        AdapterPieceTableWithLineInfo & operator=(const AdapterPieceTableWithLineInfo<T, adapter_t> & other) {
            GPT::operator=(other);
            finsert_ = other.finsert_;
            fsplit_ = other.fsplit_;
            ferase_ = other.ferase_;
            cache_length = other.cache_length;
            cache_lines = other.cache_lines;
            cache_line_start = other.cache_line_start;
            cache_line_end = other.cache_line_end;
            return *this;
        }

        protected:

        void onReset() override {
            caches.invalidate(this);
        }

        public:

        AdapterPieceTableWithLineInfo() : GPT() {
            finsert_ = this->finsert;
            fsplit_ = this->fsplit;
            ferase_ = this->ferase;

            this->finsert = [](auto * this_, auto & debug, auto & user_data, auto & start, auto & content, auto & content_length) {
                static_cast<AdapterPieceTableWithLineInfo<T, adapter_t>*>(this_)->finsert_(this_, debug, user_data, start, content, content_length);
                static_cast<AdapterPieceTableWithLineInfo<T, adapter_t>*>(this_)->caches.invalidate(this_);
            };
            this->fsplit = [](auto * this_, auto & debug, auto & user_data, auto & start, auto & length, auto & user_data_2) {
                static_cast<AdapterPieceTableWithLineInfo<T, adapter_t>*>(this_)->fsplit_(this_, debug, user_data, start, length, user_data_2);
                static_cast<AdapterPieceTableWithLineInfo<T, adapter_t>*>(this_)->caches.invalidate(this_);
            };
            this->ferase = [](auto * this_, auto & debug, auto & user_data, auto & start, auto & length, auto & is_start) {
                static_cast<AdapterPieceTableWithLineInfo<T, adapter_t>*>(this_)->ferase_(this_, debug, user_data, start, length, is_start);
                static_cast<AdapterPieceTableWithLineInfo<T, adapter_t>*>(this_)->caches.invalidate(this_);
            };
        }

        
        std::vector<std::string> lines() const {
            return this->split('\n');
        }
        
        std::size_t get_line(std::size_t pos) const {
            auto& lines = lines_cached();

            for (std::size_t i = 0, idx = 0; i < lines.size(); i++) {

                auto next_idx = idx + lines[i].size() + 1;

                if (pos < next_idx) {
                    return i;
                }

                idx = next_idx;
            }
            return 0;
        }

        std::size_t line_start(std::size_t line) const {
            auto& lines = lines_cached();

            for (std::size_t i = 0, idx = 0; i < lines.size(); i++) {

                auto next_idx = idx + lines[i].size() + 1;

                if (i == line) {
                    return idx;
                }

                idx = next_idx;
            }
            return 0;
        }

        std::size_t line_end(std::size_t line) const {
            auto& lines = lines_cached();

            for (std::size_t i = 0, idx = 0; i < lines.size(); i++) {

                auto next_idx = idx + lines[i].size() + 1;

                if (i == line) {
                    return next_idx;
                }

                idx = next_idx;
            }
            return 0;
        }

        std::size_t & length_cached() const {
            return cache_length(this);
        }

        std::vector<std::string> & lines_cached() const {
            return cache_lines(this);
        }

        std::size_t & line_start_cached(std::size_t line) const {
            return cache_line_start(this, line);
        }

        std::size_t & line_end_cached(std::size_t line) const {
            return cache_line_end(this, line);
        }

        MINIDOC_CACHE_FUNC(cache_length, AdapterPieceTableWithLineInfo::length);
        MINIDOC_CACHE_FUNC(cache_lines, AdapterPieceTableWithLineInfo::lines);
        MINIDOC_CACHE_FUNC(cache_line_start, AdapterPieceTableWithLineInfo::line_start);
        MINIDOC_CACHE_FUNC(cache_line_end, AdapterPieceTableWithLineInfo::line_end);

        CacheInvalidator caches = [](void * this_) -> std::vector<MiniDoc::CacheBase *> {
            auto * t = static_cast<AdapterPieceTableWithLineInfo*>(this_);
            return { &t->cache_lines, &t->cache_length, &t->cache_line_start, &t->cache_line_end };
        };

    };

    MINIDOC_TEMPLATE_DECL
    class MiniDoc {
        
        public:

        class Info {
            friend MiniDoc;
            AdapterPieceTableWithLineInfo<T, adapter_t> piece;
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
            
            void updateLineInfo();

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
            
            using CORE_FP = DarcsPatch::Core_FP<T, adapter_t>;
            
            template <typename FP>
            using NAMED = DarcsPatch::Named<FP, T, adapter_t>;
            
            using LAST_OP = typename AdapterPieceTableWithLineInfo<T, adapter_t>::LAST_OP;

            struct UndoInfo : public UndoStack<Info>::Command {
                LAST_OP op;
                typename AdapterPieceTableWithLineInfo<T, adapter_t>::LAST_BUFFER buffer;
                std::size_t insert_position_start;
                std::size_t replace_position_start;
                std::size_t replace_length;
                std::size_t erase_position_start;
                std::size_t erase_length;

                adapter_t content, content2;

                // patch information
                std::size_t line;
                adapter_t old_lines;
                adapter_t new_lines;

                void undo(Info * instance) override;
                void redo(Info * instance) override;
                std::ostream & to_stream(std::ostream & os, bool is_inverted) const override;
            };

            std::size_t split_count(const adapter_t & str) const;

            void append_lines(adapter_t & adapter, std::size_t count);

            void fill_patch_info(UndoInfo * undo_info);

            void record_patch(UndoInfo * undo_info);

            UndoInfo * makeUndoInfo(const adapter_t & content, const adapter_t & content2);

            void line_str(MINIDOC_STRING & out) const;
            MINIDOC_STRING line_str() const;
            
            void str(MINIDOC_STRING & out) const;
            MINIDOC_STRING str() const;
            
            void sub_str(size_t pos, size_t len, MINIDOC_STRING & out) const;
            MINIDOC_STRING sub_str(size_t pos, size_t len) const;
            
            void seek(size_t pos);
            void seek_line(size_t line);
            void seek_line_start();
            void seek_line_start(size_t line);
            void seek_line_end();
            void seek_line_end(size_t line);

            void print(std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;
            void print(const char * indent, std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;
            void printDocument(std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;
            void printDocument(const char * indent, std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;

            std::ostream & to_stream(std::ostream & os) const;

            friend std::ostream & operator<<(std::ostream & os, const Info & obj) {
                return obj.to_stream(os);
            }
        };
        
        private:
        
        mutable Info info;
        mutable UndoStack<Info> stack;
        
        public:

        const Info & get_info() const;

        std::ostream & to_stream(std::ostream & os) const;

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
        
        void line_str(MINIDOC_STRING & out) const;
        MINIDOC_STRING line_str() const;
        
        void str(MINIDOC_STRING & out) const;
        MINIDOC_STRING str() const;
        
        void sub_str(size_t pos, size_t len, MINIDOC_STRING & out) const;
        MINIDOC_STRING sub_str(size_t pos, size_t len) const;
        
        bool undo();
        bool redo();
        void set_supports_redo(bool supports_redo);
        void set_supports_advanced_undo(bool supports_advanced_undo);
        
        void append(const T * str);
        void insert(size_t pos, const T * str);
        void replace(size_t pos, const T * str);
        void replace(size_t pos, size_t len, const T * str);
        void erase(size_t pos, size_t len);
        
        void print(std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;
        
        void printDocument(std::function<void(const T* in, int*outHex, char*outChar)> conv = [](const T * in, int*outHex, char*outChar) { *outHex = (int)*in; *outChar = (char)*in; }) const;

        private:

        DarcsPatch::RL<typename Info::template NAMED<typename Info::CORE_FP>> build_patch_list() const;

        void renderDepsGraphAsDot(const DarcsPatch::DepsGraph<T, adapter_t> & g) const;

        void renderDepsGraphAsDot(const DarcsPatch::DepsGraph<T, adapter_t> & g, bool show_hashes) const;

        void renderDepsGraphAsDot(const DarcsPatch::DepsGraph<T, adapter_t> & g, bool show_hashes, bool show_names_with_hashes) const;

        void print_graph() const;

        void print_lines(std::size_t line, std::size_t column) const;

        std::optional<std::size_t> log_edit(uint64_t edit_id) const;

        void renderDepsGraphAsBacktrace(const DarcsPatch::DepsGraph<T, adapter_t> & g, const std::string & message) const;

        public:

        void error() const;

        void error(const std::string & message) const;

        friend std::ostream & operator<<(std::ostream & os, const MiniDoc<T, adapter_t> & obj) {
            return obj.to_stream(os);
        }
    };
    
    using MiniDoc_T = MiniDoc<char, StringAdapter::CharAdapter>;
}

// source>

#include <stdlib.h> // malloc.h is not portable
#include <cstring> // strlen

namespace MiniDoc {
    MINIDOC_TEMPLATE_IMPL
    MINIDOC_TEMPLATE_DEF::MiniDoc() {
        load(nullptr);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::load(std::nullptr_t stream) {
        load(nullptr, 0);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::load(std::nullptr_t stream, size_t length) {
        auto a = adapter_t();
        static const T empty[1] = { a.get_end_of_file() };
        load(empty, length);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::load(const T* stream) {
        std::size_t length = 0;
        auto a = adapter_t();
        const T & eof = a.get_end_of_file();
        while(true) {
            if (stream[length] == eof) {
                break;
            }
            length++;
        }
        load(stream, length);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::load(const T* stream, size_t length) {
        info = std::move(Info());
        stack = std::move(UndoStack<Info>());

        if (length != 0) {
            auto a = adapter_t(stream, length);
            auto data = a.data();
            info.piece.append_origin(data.ptr());
        }
        info.updateLineInfo();
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::append(const T * str) {
        insert(-1, str);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::insert(size_t pos, const T * str) {
        info.piece.insert(str, pos);
        info.updateLineInfo();
        stack.push(info.makeUndoInfo(str, ""));
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::replace(size_t pos, const T * str) {
        replace(pos, 1, str);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::replace(size_t pos, size_t len, const T * str) {
        auto erased = info.piece.range_string_adapter_len(pos, len);
        info.piece.replace(str, pos, len);
        info.updateLineInfo();
        stack.push(info.makeUndoInfo(erased, str));
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::erase(size_t pos, size_t len) {
        auto erased = info.piece.range_string_adapter_len(pos, len);
        info.piece.erase(pos, len);
        info.updateLineInfo();
        stack.push(info.makeUndoInfo(erased, ""));
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::updateLineInfo() {
        length_ = piece.length_cached();
        if (cursor_ > length_) {
            cursor_ = length_;
        }
        line_ = piece.get_line(cursor_);
        lines_ = piece.lines_cached().size();
        line_start_ = piece.line_start_cached(line_);
        line_end_ = piece.line_end_cached(line_);
        line_length_ = (line_end_) - line_start_;
        if (line_ == lines_) {
            line_end_--;
            line_length_--;
        }
        column_ = cursor_ - line_start_;
        character_ = length_ == 0 ? '\0' : cursor_ == length_ ? '\0' : piece[cursor_];
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::seek(size_t pos) {
        if (pos == cursor_) {
            return;
        }
        auto l = (length_ == 0 ? 0 : length_);
        if (pos == -1 || pos > l) {
            pos = l;
        }
        cursor_ = pos;
        updateLineInfo();
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::seek(size_t pos) {
        info.seek(pos);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::seek_line(size_t line) {
        if (line == line_) {
            return;
        }
        auto l = lines_-1;
        if (line == -1) {
            line = l;
        }
        if (line > l) {
            line = l;
        }
        if (line > line_) {
            while (line != line_) {
                seek(line_end_);
            }
        } else {
            while (line != line_) {
                seek(line_start_-1);
                seek(line_start_);
            }
        }
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::seek_line(size_t line) {
        info.seek_line(line);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::seek_line_start() {
        seek(line_start_);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::seek_line_start(size_t line) {
        seek_line(line);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::seek_line_end() {
        seek(line_end_-1);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::seek_line_end(size_t line) {
        seek_line(line);
        seek_line_end();
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::seek_line_start() {
        info.seek_line_start();
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::seek_line_start(size_t line) {
        info.seek_line_start(line);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::seek_line_end() {
        info.seek_line_end();
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::seek_line_end(size_t line) {
        info.seek_line_end(line);
    }
    
    MINIDOC_TEMPLATE_IMPL
    bool MINIDOC_TEMPLATE_DEF::has_next() const {
        return info.cursor_ != (info.length_ == 0 ? 0 : info.length_);
    }
    
    MINIDOC_TEMPLATE_IMPL
    T MINIDOC_TEMPLATE_DEF::next() {
        T ch = character();
        if (has_next()) {
            info.cursor_++;
            info.updateLineInfo();
        }
        return ch;
    }
    
    MINIDOC_TEMPLATE_IMPL
    bool MINIDOC_TEMPLATE_DEF::has_previous() const {
        return info.cursor_ != 0;
    }
    
    MINIDOC_TEMPLATE_IMPL
    T MINIDOC_TEMPLATE_DEF::previous() {
        T ch = character();
        if (has_previous()) {
            info.cursor_--;
            info.updateLineInfo();
        }
        return ch;
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::print(std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        print("", conv);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::print(const char * indent, std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        const char * i = indent == nullptr ? "" : indent;
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
        auto c_str = s.c_str();
        printf("%sline str: %s\n", i, c_str.ptr());
        printf("%sline str size: %zu\n", i, c_str.length());
        printf("%sline str hex\n", i);
        std::cout << CustomHexdump<8, true, char>("        ", c_str.ptr(), c_str.length()) << std::endl;
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
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::printDocument(std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        printDocument("", conv);
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::printDocument(const char * indent, std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        const char * i = indent == nullptr ? "" : indent;
        printf("%slines: %zu\n", i, lines_);
        printf("%slength: %zu\n", i, length_);
        auto s = str();
        auto c_str = s.c_str();
        printf("%sdocument:\n%s\n", i, c_str.ptr());
        printf("%sdocument len: %zu\n", i, c_str.length());
        printf("%sdocument hex\n", i);
        std::cout << CustomHexdump<8, true, char>("        ", c_str.ptr(), c_str.length(), conv) << std::endl;
    }
    
    MINIDOC_TEMPLATE_IMPL
    T MINIDOC_TEMPLATE_DEF::Info::character() const {
        return character_;
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::Info::cursor() const {
        return cursor_;
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::Info::line_start() const {
        return line_start_;
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::Info::line_end() const {
        return line_end_;
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::Info::line_length() const {
        return line_length_;
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::Info::line() const {
        return line_;
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::Info::lines() const {
        return lines_;
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::Info::column() const {
        return column_;
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::Info::length() const {
        return length_;
    }
    MINIDOC_TEMPLATE_IMPL
    T MINIDOC_TEMPLATE_DEF::character() const {
        return info.character();
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::cursor() const {
        return info.cursor();
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::line_start() const {
        return info.line_start();
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::line_end() const {
        return info.line_end();
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::line_length() const {
        return info.line_length();
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::line() const {
        return info.line();
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::lines() const {
        return info.lines();
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::column() const {
        return info.column();
    }
    MINIDOC_TEMPLATE_IMPL
    size_t MINIDOC_TEMPLATE_DEF::length() const {
        return info.length();
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::line_str(MINIDOC_STRING & out) const {
        piece.range_string_adapter(line_start_, line_end_, out);
    }
    MINIDOC_TEMPLATE_IMPL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::Info::line_str() const {
        MINIDOC_STRING s;
        line_str(s);
        return s;
    }
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::line_str(MINIDOC_STRING & out) const {
        info.line_str(out);
    }
    MINIDOC_TEMPLATE_IMPL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::line_str() const {
        MINIDOC_STRING s;
        line_str(s);
        return s;
    }
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::str(MINIDOC_STRING & out) const {
        piece.range_string_adapter_len(0, length_, out);
    }
    MINIDOC_TEMPLATE_IMPL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::Info::str() const {
        MINIDOC_STRING s;
        str(s);
        return s;
    }
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::str(MINIDOC_STRING & out) const {
        info.str(out);
    }
    MINIDOC_TEMPLATE_IMPL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::str() const {
        MINIDOC_STRING s;
        str(s);
        return s;
    }
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::sub_str(size_t pos, size_t len, MINIDOC_STRING & out) const {
        auto p = pos == -1 ? length_ : pos >= length_ ? length_ : pos;
        piece.range_string_adapter_len(p, len, out);
    }
    MINIDOC_TEMPLATE_IMPL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::Info::sub_str(size_t pos, size_t len) const {
        MINIDOC_STRING s;
        sub_str(pos, len, s);
        return s;
    }
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::sub_str(size_t pos, size_t len, MINIDOC_STRING & out) const {
        info.sub_str(pos, len, out);
    }
    MINIDOC_TEMPLATE_IMPL
    MINIDOC_STRING MINIDOC_TEMPLATE_DEF::sub_str(size_t pos, size_t len) const {
        MINIDOC_STRING s;
        sub_str(pos, len, s);
        
        return s;
    }
    
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::UndoInfo::undo(Info * instance) {
        if (op == LAST_OP::LAST_OP_INSERT) {
            instance->piece.erase(insert_position_start, content.length());
            instance->updateLineInfo();
        } else if (op == LAST_OP::LAST_OP_REPLACE) {
            auto shared = content.data();
            instance->piece.replace(shared.ptr(), replace_position_start, content2.size());
            instance->updateLineInfo();
        } else if (op == LAST_OP::LAST_OP_ERASE) {
            auto shared = content.data();
            instance->piece.insert(shared.ptr(), erase_position_start);
            instance->updateLineInfo();
        }
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::UndoInfo::redo(Info * instance) {
        if (op == LAST_OP::LAST_OP_INSERT) {
            auto shared = content.data();
            instance->piece.insert(shared.ptr(), insert_position_start);
            instance->updateLineInfo();
        } else if (op == LAST_OP::LAST_OP_REPLACE) {
            auto shared = content2.data();
            instance->piece.replace(shared.ptr(), replace_position_start, replace_length);
            instance->updateLineInfo();
        } else if (op == LAST_OP::LAST_OP_ERASE) {
            instance->piece.erase(erase_position_start, erase_length);
            instance->updateLineInfo();
        }
    }

    MINIDOC_TEMPLATE_IMPL
    std::ostream & MINIDOC_TEMPLATE_DEF::Info::UndoInfo::to_stream(std::ostream & os, bool is_inverted) const {
        return os << "Minidoc Command: content: \"" << escape<T, adapter_t>(content) << "\", content2: \"" << escape<T, adapter_t>(content2) << "\"";
    }

    MINIDOC_TEMPLATE_IMPL
    std::size_t MINIDOC_TEMPLATE_DEF::Info::split_count(const adapter_t & str) const {
        std::size_t count = 0;
        const auto & nl = str.get_new_line();
        for (const auto & c : str) {
            if (c == nl) count++;
        }
        return count;
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::append_lines(adapter_t & adapter, std::size_t count) {
        auto & nl = adapter.get_new_line();

        for (std::size_t i = 0; i < count; i++) {
            adapter.append(nl);
        }
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::fill_patch_info(UndoInfo * undo_info) {
        // split_count(content) :
        // content = "abcde" // line count 0
        // content = "abcd\ne" // line count 1
        // content = "ab\ncd\ne" // line count 2

        if (undo_info->op == LAST_OP::LAST_OP_INSERT) {
            // record an insertion
            //
            // content: ABC
            // insert "\nege\neweg"
            // content: AB\nege\newegC
            //
            undo_info->line = piece.get_line(undo_info->insert_position_start);

            // we know our old line has a line count of zero, and old_line is already empty

            // our content could add new lines
            //
            append_lines(undo_info->new_lines, split_count(undo_info->content));
        } else if (undo_info->op == LAST_OP::LAST_OP_REPLACE) {
            // record a replacement
            //
            //
            // content: AB\nege\newegC
            // replace ""
            // content: ABC
            // -- OR --
            // content: ABC
            // replace "\nege\neweg"
            // content: A\nege\newegC

            // to undo a replacement, we replace the new contents with the old contents
            // in the context of a patch, this translates to old line = old contents, new line = new contents

            undo_info->line = piece.get_line(undo_info->replace_position_start);

            // the old line count is our old content
            append_lines(undo_info->old_lines, split_count(undo_info->content));
            // the new line count is our new contents
            append_lines(undo_info->new_lines, split_count(undo_info->content2));
        } else if (undo_info->op == LAST_OP::LAST_OP_ERASE) {
            // record an erasure
            //
            // content: AB\nege\newegC
            // erase "\nege\neweg"
            // content: ABC
            //
            undo_info->line = piece.get_line(undo_info->erase_position_start);

            // we know our new line has a line count of zero, and new_line is already empty

            // our erasure content could remove new lines
            append_lines(undo_info->old_lines, split_count(undo_info->content));
        }
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::Info::record_patch(UndoInfo * undo_info) {
        // we require the line number, line count before, and line count after
        //
        // the cursor has not changed, but the line content before and after has changed
        // all edits may span multiple lines
        //
        // assume edits cannot change content that occurs before our edit position
        //
        fill_patch_info(undo_info);
    }

    MINIDOC_TEMPLATE_IMPL
    typename MINIDOC_TEMPLATE_DEF::Info::UndoInfo * MINIDOC_TEMPLATE_DEF::Info::makeUndoInfo(const adapter_t & content, const adapter_t & content2) {
        UndoInfo * undo_info = new UndoInfo();
        undo_info->op = piece.last_op;
        undo_info->buffer = piece.last_buffer;
        undo_info->insert_position_start = piece.last_calculated_insert_position_start;
        undo_info->replace_position_start = piece.last_calculated_replace_position_start;
        undo_info->replace_length = piece.last_calculated_replace_length;
        undo_info->erase_position_start = piece.last_calculated_erase_position_start;
        undo_info->erase_length = piece.last_calculated_erase_length;
        undo_info->content = content;
        undo_info->content2 = content2;
        record_patch(undo_info);
        return undo_info;
    }

    MINIDOC_TEMPLATE_IMPL
    std::ostream & MINIDOC_TEMPLATE_DEF::Info::to_stream(std::ostream & os) const {
        os << "MiniDoc::Info start" << std::endl;
        os << "    tag: " << tag << std::endl;
        os << "    character_: " << character_ << std::endl;
        os << "    cursor_: " << std::to_string(cursor_) << std::endl;
        os << "    line_: " << std::to_string(line_) << std::endl;
        os << "    lines_: " << std::to_string(lines_) << std::endl;
        os << "    line_start__: " << std::to_string(line_start_) << std::endl;
        os << "    line_end_: " << std::to_string(line_end_) << std::endl;
        os << "    line_length_: " << std::to_string(line_length_) << std::endl;
        os << "    column_: " << std::to_string(column_) << std::endl;
        os << "    length_: " << std::to_string(length_) << std::endl;
        os << "    piece table: " << std::endl;
        os << piece << std::endl;
        return os << "MiniDoc::Info end";
    }

    MINIDOC_TEMPLATE_IMPL
    const typename MINIDOC_TEMPLATE_DEF::Info & MINIDOC_TEMPLATE_DEF::get_info() const { return info; }

    MINIDOC_TEMPLATE_IMPL
    std::ostream & MINIDOC_TEMPLATE_DEF::to_stream(std::ostream & os) const {
        os << "MiniDoc start" << std::endl;
        os << "    info: " << std::endl;
        return info.to_stream(os) << std::endl << "    undo stack: " << stack << std::endl << "MiniDoc end";
    }

    MINIDOC_TEMPLATE_IMPL
    bool MINIDOC_TEMPLATE_DEF::undo() {
        return stack.undo(&info);
    }
    MINIDOC_TEMPLATE_IMPL
    bool MINIDOC_TEMPLATE_DEF::redo() {
        return stack.redo(&info);
    }
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::set_supports_redo(bool supports_redo) {
        stack.supports_redo = supports_redo;
    }
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::set_supports_advanced_undo(bool supports_advanced_undo) {
        stack.supports_advanced_undo = supports_advanced_undo;
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::print(std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        info.print(conv);
        std::cout << stack << std::endl;
        printf("\n");
        // print_graph();
        // printf("\n");
    }
    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::printDocument(std::function<void(const T* in, int*outHex, char*outChar)> conv) const {
        info.printDocument(conv);
        printf("\n");
    }

    MINIDOC_TEMPLATE_IMPL
    DarcsPatch::RL<typename MINIDOC_TEMPLATE_DEF::Info::template NAMED<typename MINIDOC_TEMPLATE_DEF::Info::CORE_FP>> MINIDOC_TEMPLATE_DEF::build_patch_list() const {

        auto digits = numDigits(stack.undoSize());

        DarcsPatch::RL<typename MINIDOC_TEMPLATE_DEF::Info::template NAMED<typename MINIDOC_TEMPLATE_DEF::Info::CORE_FP>> patch_list;
        std::size_t patch_id = 0;

        std::string zero;
        zero.append(digits, '0');

        patch_list = patch_list.push(DarcsPatch::makeNamedWithType<T, adapter_t>(0, zero.c_str(), DarcsPatch::makeAddFile()));

        stack.transform_undo_stack([&](auto cmd) {
            patch_id++;
            std::string num;
            num.append(digits - numDigits(patch_id), '0');
            num.append(std::to_string(patch_id));

            bool inverted = cmd->is_inverted();
            const typename Info::UndoInfo * command = static_cast<const typename Info::UndoInfo*>(inverted ? cmd->get_command() : cmd);
            auto hunk = DarcsPatch::makeHunk<T, adapter_t>(command->line, command->old_lines, command->new_lines);
            if (inverted) {
                hunk = hunk->invert();
            }
            patch_list = patch_list.push(DarcsPatch::makeNamedWithType<T, adapter_t>(patch_id, num, hunk));
        });
        return patch_list;
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::renderDepsGraphAsDot(const DarcsPatch::DepsGraph<T, adapter_t> & g) const {
        renderDepsGraphAsDot(g, false, true);
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::renderDepsGraphAsDot(const DarcsPatch::DepsGraph<T, adapter_t> & g, bool show_hashes) const {
        renderDepsGraphAsDot(g, false, show_hashes);
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::renderDepsGraphAsDot(const DarcsPatch::DepsGraph<T, adapter_t> & g, bool show_hashes, bool show_names_with_hashes) const {
        std::cout << "digraph {";
        auto indent = "   ";
        std::cout << "\n" << indent << "graph [rankdir=LR];";
        std::cout << "\n" << indent << "node [imagescale=true];";

        auto showID = [] (auto & key) {
            SHA1 sha1;
            auto & n = key.name.c_str();
            return sha1(n.ptr(), n.lengthInBytes());
        };

        auto showNode = [&](auto & key) {
            std::cout << "\n" << indent << "\"" << showID(key) << "\" [label=" << key.name << "]";
        };

        auto showEdges = [&](auto & key, auto & value) {
            auto begin = value.begin();
            if (begin != value.end()) {
                if (show_hashes) {
                    std::cout << "\n" << indent << "\"" << showID(key) << "\"";
                    if (show_names_with_hashes) {
                        std::cout << " [label=" << key.name << "]";
                    }
                    std::cout << " -> " << "{\"" << showID(*begin) << "\"";
                    if (show_names_with_hashes) {
                        std::cout << " [label=" << (*begin).name << "]";
                    }
                    std::cout << "}";
                } else {
                    std::cout << "\n" << indent << key.name << " -> " << (*begin).name;
                }
            }
        };

        bool printed = false;

        for(auto & pair : g) {
            printed = true;
            showNode(pair.first);
        }

        for(auto & pair : g) {
            printed = true;
            showEdges(pair.first, pair.second.v1);
        }

        if (printed) {
            std::cout << "\n";
        }

        std::cout << "}\n";
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::print_graph() const {
        auto graph = DarcsPatch::depsGraph<T, adapter_t>(build_patch_list());
        renderDepsGraphAsDot(graph, false);
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::print_lines(std::size_t line, std::size_t column) const {
        auto print_line = [&](std::size_t l, std::size_t col, bool print_cursor) {
            info.seek_line(l);
            auto s = line_str();
            auto c_str = s.c_str();
            std::string prefix = std::string("   ");
            if (print_cursor) {
                prefix += "at";
            } else {
                prefix += "  ";
            }
            prefix += " line ";
            prefix += std::to_string(l) + ", column " + std::to_string(col) + " : ";
            std::string escaped = escape(c_str.ptr(), c_str.length());
            std::cout << prefix << "\"" << escaped << "\"" << std::endl;
            if (print_cursor) {
                std::string c;
                c.append(prefix.size()+1+escape_index(c_str.ptr(), c_str.length(), col), ' ');
                c.push_back('^');
                std::cout << c << std::endl;
            }
        };
        auto lines = info.lines_-1;
        if (line == 0) {
            print_line(line, column, true);
            if (lines > 1) {
                print_line(line+1, 0, false);
                std::cout << std::endl;
            }
        } else if (line == 1) {
            print_line(line-1, 0, false);
            print_line(line, column, true);
            if (lines > 2) {
                print_line(line+1, 0, false);
                std::cout << std::endl;
            }
        } else {
            print_line(line-1, 0, false);
            print_line(line, column, true);
            if (line != lines) {
                print_line(line+1, 0, false);
                std::cout << std::endl;
            }
        }
    }

    MINIDOC_TEMPLATE_IMPL
    std::optional<std::size_t> MINIDOC_TEMPLATE_DEF::log_edit(uint64_t edit_id) const {
        auto edit = stack.get_index(edit_id);
        if (edit == nullptr) {
            return std::nullopt;
        }
        bool inverted = edit->is_inverted();
        const typename Info::UndoInfo * u = static_cast<const typename Info::UndoInfo*>(inverted ? edit->get_command() : edit);
        using LAST_OP = typename Info::LAST_OP;
        auto cursor = info.cursor_;
        if (inverted) {
            if (u->op == LAST_OP::LAST_OP_INSERT) {
                std::cout << " before erase: \"" << escape<T, adapter_t>(u->content) << "\"" << std::endl;
                info.cursor_ = u->insert_position_start;
                info.updateLineInfo();
            } else if (u->op == LAST_OP::LAST_OP_REPLACE) {
                std::cout << " before replace: \"" << escape<T, adapter_t>(u->content2) << "\"" << " with \"" << escape<T, adapter_t>(u->content) << "\"" << std::endl;
                info.cursor_ = u->replace_position_start;
                info.updateLineInfo();
            } else if (u->op == LAST_OP::LAST_OP_ERASE) {
                std::cout << " before insert: \"" << escape<T, adapter_t>(u->content) << "\"" << std::endl;
                info.cursor_ = u->erase_position_start;
                info.updateLineInfo();
            }
        } else {
            if (u->op == LAST_OP::LAST_OP_INSERT) {
                std::cout << " before insert: \"" << escape<T, adapter_t>(u->content) << "\"" << std::endl;
                info.cursor_ = u->insert_position_start;
                info.updateLineInfo();
            } else if (u->op == LAST_OP::LAST_OP_REPLACE) {
                std::cout << " before replace: \"" << escape<T, adapter_t>(u->content) << "\"" << " with \"" << escape<T, adapter_t>(u->content2) << "\"" << std::endl;
                info.cursor_ = u->replace_position_start;
                info.updateLineInfo();
            } else if (u->op == LAST_OP::LAST_OP_ERASE) {
                std::cout << " before erase: \"" << escape<T, adapter_t>(u->content) << "\"" << std::endl;
                info.cursor_ = u->erase_position_start;
                info.updateLineInfo();
            }
        }
        auto k = stack.undo_to_index(edit_id, &info);
        auto line = info.line_;
        auto column = info.column_;
        print_lines(line, column);
        info.cursor_ = cursor;
        info.updateLineInfo();
        return {k};
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::renderDepsGraphAsBacktrace(const DarcsPatch::DepsGraph<T, adapter_t> & g, const std::string & message) const {
        auto cursor = info.cursor_;
        {
            if (message.length() == 0) {
                std::cout << "An error has occured" << std::endl;
            } else {
                std::cout << message << std::endl;
            }
            auto line = info.line_;
            auto column = info.column_;
            print_lines(line, column);
            info.cursor_ = cursor;
            info.updateLineInfo();
            std::cout << "printing backtrace..." << std::endl;
        }
        auto begin = g.begin();
        auto end = g.end();
        bool target_edit_found = false;
        LOOP:
        if (begin != end) {
            end--;
            auto pair = *end;
            auto begin = pair.second.v1.begin();
            if (begin == pair.second.v1.end()) {
                goto LOOP;
            }
            if (!target_edit_found) {
                auto edit = stack.get_index(pair.first.additional_data-1);
                if (edit == nullptr) {
                    goto LOOP;
                }
                bool inverted = edit->is_inverted();
                const typename Info::UndoInfo * u = static_cast<const typename Info::UndoInfo*>(inverted ? edit->get_command() : edit);
                if (info.line_ >= u->line) {
                    auto hunk = DarcsPatch::makeHunk<T, adapter_t>(u->line, u->old_lines, u->new_lines);
                    if(inverted) {
                        hunk = hunk->invert();
                    }
                    auto fp = std::static_pointer_cast<DarcsPatch::FileHunk<T, adapter_t>>(hunk);
                    if (info.line_ < u->line + fp->new_lines.size()) {
                        target_edit_found = true;
                    } else {
                        goto LOOP;
                    }
                } else {
                    goto LOOP;
                }
            }
            std::cout << " edit #" << std::to_string(pair.first.additional_data) << ":";
            auto saved = info.cursor_;
            auto k = log_edit(pair.first.additional_data-1);
            LOOP_START:
            std::cout << " edit #" << std::to_string(begin->additional_data) << ":";
            if (begin->additional_data == 0) {
                std::cout << " ADD FILE" << std::endl;
            } else {
                log_edit(begin->additional_data-1);
                for(auto & pair2 : g) {
                    auto begin2 = pair2.second.v1.begin();
                    if (begin2 != pair2.second.v1.end()) {
                        if (begin->name == pair2.first.name) {
                            begin = begin2;
                            goto LOOP_START;
                        }
                    }
                }
            }
            std::cout << "end of backtrace" << std::endl;
            if (k.has_value()) {
                stack.redo_to_index(k.value(), &info);
            }
        }
        info.seek(cursor);
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::error() const {
        error("");
    }

    MINIDOC_TEMPLATE_IMPL
    void MINIDOC_TEMPLATE_DEF::error(const std::string & message) const {
        auto graph = DarcsPatch::depsGraph<T, adapter_t>(build_patch_list());
        renderDepsGraphAsBacktrace(graph, message);
    }
}

#endif