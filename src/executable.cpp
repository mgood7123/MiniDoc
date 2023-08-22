#include <minidoc.h>

struct CharHolderEncoded :
        public StringAdapter::Comparable<CharHolderEncoded>,
        public StringAdapter::Hashable<CharHolderEncoded>
{
    COMPARABLE_USING_BASE(StringAdapter::Comparable<CharHolderEncoded>);
    HASHABLE_USING_BASE(StringAdapter::Hashable<CharHolderEncoded>);
    int c;
    CharHolderEncoded() : CharHolderEncoded('\0') {}
    CharHolderEncoded(const char c) :
        c(c - '0'),
        CharHolderEncoded::Comparable([](auto & a, auto & b) { return compare_3(a, b, &CharHolderEncoded::c); }),
        CharHolderEncoded::Hashable([](auto & a) { return hash_3(a, &CharHolderEncoded::c); })
    {}
    const char decode() const { return c + '0'; }

    operator char() const { return decode(); }
};

STRING_ADAPTER_HASHCODE_SPEC(CharHolderEncoded);

::std::ostream& operator <<(::std::ostream& os, const CharHolderEncoded & this_) {
    return os << this_.decode();
}

::std::ostream& operator <<(::std::ostream& os, const CharHolderEncoded * this_) {
    for(std::size_t i = 0; ; i++) {
        const char c = this_[i].decode();
        if (c == '\0') {
            break;
        }
        os << c;
    }
    return os;
}

struct CharHolder : public StringAdapter::VectorAdapter<CharHolderEncoded> {
    using BASE = StringAdapter::VectorAdapter<CharHolderEncoded>;
    BASIC_STRING_ADAPTER_USING(BASE);

    CharHolder() : BASE({'\n'}, {'\0'}) {}

    static StringAdapter::Shared<char> convert(const StringAdapter::Shared<CharHolderEncoded> & ptr) {
        char * p = new char[ptr.length()+1];
        for (std::size_t i = 0; i < ptr.length(); i++) {
            p[i] = ptr.ptr()[i].decode();
        }
        p[ptr.length()] = '\0';
        return {p, ptr.length(), [](auto p) { delete[] p; }};
    }

    static const StringAdapter::CShared<char> convert(const StringAdapter::CShared<CharHolderEncoded> & ptr) {
        char * p = new char[ptr.length()+1];
        for (std::size_t i = 0; i < ptr.length(); i++) {
            p[i] = ptr.ptr()[i].decode();
        }
        p[ptr.length()] = '\0';
        return {p, ptr.length(), [](auto p) { delete[] p; }};
    }

    static const StringAdapter::CShared<CharHolderEncoded> convert(const char* ptr, std::size_t len) {
        CharHolderEncoded * p = new CharHolderEncoded[len+1];
        for (std::size_t i = 0; i < len; i++) {
            p[i] = {ptr[i]};
        }
        p[len] = {'\0'};
        return {p, len, [](auto p) { delete[] p; }};
    }

    static const StringAdapter::CShared<CharHolderEncoded> convert(const char* ptr) {
        return convert(ptr, std::strlen(ptr));
    }

    CharHolder(const StringAdapter::CShared<CharHolderEncoded> & ptr) : BASE(ptr, {'\n'}, {'\0'}) {}

    CharHolder(const char * ptr) : BASE(convert(ptr), {'\n'}, {'\0'}) {}

    CharHolder(const char * ptr, const std::size_t length) : BASE(convert(ptr, length), length, '\n', '\0') {}

    CharHolder(const CharHolderEncoded * ptr) : BASE(ptr, {'\n'}, {'\0'}) {}

    CharHolder(const CharHolderEncoded * ptr, const std::size_t length) : BASE(ptr, length, '\n', '\0') {}

    CharHolder(const std::string & str) : CharHolder(convert(str.data(), str.size())) {}

    void print(const char* str) const {
        const std::size_t len = size();
        std::cout << str << ". (size (excluding null): " << len << ") ";
        const StringAdapter::CShared<CharHolderEncoded> data_ = data();
        for (std::size_t i = 0; i <= len; i++) {
            if (data_.ptr()[i] == get_end_of_file()) {
                std::cout << "\\0" << ' ';
            } else {
                std::cout << data_.ptr()[i].decode() << ' ';
            }
        }
        std::cout << '\n';
    }

    void append(const std::string & s) {
        append(s.data(), s.length());
    }

    void append(const char * s) {
        append_(CharHolder(s, strlen(s)));
    }

    void append(const CharHolderEncoded * s) {
        append_(CharHolder(s));
    }

    void append(const char * s, std::size_t len) {
        append_(CharHolder(s, len));
    }

    void append(const CharHolderEncoded * s, std::size_t len) {
        append_(CharHolder(s, len));
    }

    void append(const char & c) {
        CharHolderEncoded s[2] = { {c}, get_end_of_file() };
        append_(CharHolder(s, 1));
    }

    void append(const CharHolderEncoded & c) {
        CharHolderEncoded s[2] = { c, get_end_of_file() };
        append_(CharHolder(s, 1));
    }


    void insert(const std::size_t pos, const std::string & s) {
        insert_(CharHolder(s.data(), s.length()), pos);
    }

    void insert(const std::size_t pos, const char* s) {
        insert_(CharHolder(s, strlen(s)), pos);
    }

    void insert(const std::size_t pos, const CharHolderEncoded* s) {
        insert_(CharHolder(s), pos);
    }

    void insert(const std::size_t pos, const char* s, const std::size_t len2) {
        insert_(CharHolder(s, len2), pos);
    }

    void insert(const std::size_t pos, const char & c) {
        CharHolderEncoded s[2] = { {c}, get_end_of_file() };
        insert_(CharHolder(s, 1), pos);
    }

    void insert(const std::size_t pos, const CharHolderEncoded & c) {
        CharHolderEncoded s[2] = { c, get_end_of_file() };
        insert_(CharHolder(s, 1), pos);
    }


    void replace(const std::size_t pos, const std::size_t len, const std::string & s) {
        replace_(CharHolder(s.data(), s.length()), pos, len);
    }

    void replace(const std::size_t pos, const std::size_t len, const char* s) {
        replace_(CharHolder(s, strlen(s)), pos, len);
    }

    void replace(const std::size_t pos, const std::size_t len, const CharHolderEncoded* s) {
        replace_(CharHolder(s), pos, len);
    }

    void replace(const std::size_t pos, const std::size_t len, const char* s, const std::size_t len2) {
        replace_(CharHolder(s, len2), pos, len);
    }

    void replace(const std::size_t pos, const std::size_t len, const char & c) {
        CharHolderEncoded s[2] = { {c}, get_end_of_file() };
        replace_(CharHolder(s, 1), pos, len);
    }

    void replace(const std::size_t pos, const std::size_t len, const CharHolderEncoded & c) {
        CharHolderEncoded s[2] = { c, get_end_of_file() };
        replace_(CharHolder(s, 1), pos, len);
    }

    void erase(const std::size_t pos, const std::size_t len) {
        erase_(pos, len);
    }

    const char index_to_char(const std::size_t index) const override {
        return operator[](index).decode();
    }

    StringAdapter::Shared<char> c_str() override {
        return convert(c_str_());
    }

    const StringAdapter::CShared<char> c_str() const override {
        return convert(c_str_());
    }

    const void deleteSelf() const override {
        delete (CharHolder*) this;
    }

    struct Slice : public BASE::Slice {
        using BASE_S = typename BASE::Slice;
        BASIC_STRING_ADAPTER_SLICE_USING_BASE(BASE_S)
        CharHolder* get_origin() override {
            return reinterpret_cast<CharHolder*>(get_origin_());
        };
    };

    struct CSlice : public BASE::CSlice {
        using BASE_S = typename BASE::CSlice;
        BASIC_STRING_ADAPTER_SLICE_USING_BASE(BASE_S)
        const CharHolder* get_origin() override {
            return reinterpret_cast<const CharHolder*>(get_origin_());
        };
    };

    Slice* slice(std::size_t start, std::size_t end) override {
        return new Slice(this, (begin()+start).index, (begin()+end).index);
    };

    const CSlice* cslice(std::size_t start, std::size_t end) const override {
        return new CSlice(this, (begin()+start).index, (begin()+end).index);
    };

    const CSlice* slice(std::size_t start, std::size_t end) const override {
        return new CSlice(this, (begin()+start).index, (begin()+end).index);
    };

    CharHolder* clone() override {
        return new CharHolder(*this);
    }

    const CharHolder* clone() const override {
        return new CharHolder(*this);
    }

    CharHolder* newAdapter() override {
        return new CharHolder();
    }

    CharHolder* newAdapter(const CharHolderEncoded * ptr) override {
        return new CharHolder(ptr);
    }

    CharHolder* newAdapter(const CharHolderEncoded * ptr, const std::size_t length) override {
        return new CharHolder(ptr, length);
    }

    CharHolder* newAdapter() const override {
        return new CharHolder();
    }

    CharHolder* newAdapter(const CharHolderEncoded * ptr) const override {
        return new CharHolder(ptr);
    }

    CharHolder* newAdapter(const CharHolderEncoded * ptr, const std::size_t length) const override {
        return new CharHolder(ptr, length);
    }
};

STRING_ADAPTER_HASHCODE_SPEC(CharHolder);

int main() {
    printf("hi\n");

    using M = MiniDoc::MiniDoc_T;

    M m;

    printf("sizeof MiniDoc: %zu\n", sizeof(M));
    printf("sizeof MiniDoc::Info: %zu\n", sizeof(M::Info));

    StringAdapter::CharAdapter s;
    s.append("1");
    auto c_str = s.c_str();
    auto i = "  ";
    printf("%sline str: %s\n", i, c_str.ptr());
    printf("%sline str size: %zu\n", i, c_str.length());
    printf("%sline str hex\n", i);
    std::cout << MiniDoc::CustomHexdump<8, true, char>("        ", c_str.ptr(), c_str.length()) << std::endl;
    //  exit(0);

    m.load("");
    m.print();
    // exit(0);
    m.load("");
    m.append("1");
    m.print();
    // exit(0);
    m.load("1");
    m.print();
    // exit(0);
    m.load("1234");
    m.print();
    // exit(0);
    m.erase(1, 1);
    m.print();
    m.load("1234");
    m.print();
    // exit(0);
    m.replace(1, "47"); // 1234 -> 14734
    m.print();
    m.replace(1, "86"); // 14734 -> 186734
    m.print();
    m.replace(1, 3, "55"); // 186734 -> 15534
    m.print();

    m.load("");
    m.print();
    m.append("A");
    m.print();
    puts("undo");
    m.undo();
    m.print();
    puts("redo");
    m.redo();
    m.print();

    m.load("");
    m.print();
    m.append("A");
    m.append("B");
    m.append("C");
    m.print();
    puts("undo");
    m.undo();
    m.print();
    puts("undo");
    m.undo();
    m.print();
    puts("undo");
    m.undo();
    m.print();
    puts("append X");
    m.append("X");
    m.print();
    puts("undo");
    m.undo();
    m.print();
    puts("undo");
    m.undo();
    m.print();
    puts("undo");
    m.undo();
    m.print();
    puts("undo");
    m.undo();
    m.print();
    puts("undo");
    m.undo();
    m.print();
    puts("undo");
    m.undo();
    m.print();
    puts("undo");
    m.undo();
    m.print();

    m.load("apple");
    m.append("ban");
    m.print();

    m.load("apple");
    m.insert(-1, "ban");
    m.print();

    m.load("apple");
    m.insert(1, "ban");
    m.print();

    m.load("apple");
    m.replace(-1, "ban");
    m.print();

    m.load("apple");
    m.replace(1, "ban");
    m.print();

    m.load("apple");
    m.erase(1, 3);
    m.print();

    m.load("apple");
    m.erase(1, -1);
    m.print();

    printf("\n\n\nhttp://darcs.net/Using/Model#hunk-patches\n\n");
    m.load("");
    m.append("all\nwhere\noccupied");
    m.printDocument();
    m.seek_line_start(1);
    m.insert(m.cursor(), "the\nseats\n");
    m.printDocument();
    m.seek_line_start(2);
    m.insert(m.cursor(), "clean\n");
    m.printDocument();
    m.seek_line_start(5);
    m.insert(m.cursor(), "duly\n");
    m.printDocument();
    m.seek_line_start(3);
    m.insert(m.cursor(), "blue\n");
    m.printDocument();
    m.seek_line_start(4);
    m.replace(m.cursor(), m.line_length()-1, "tables");
    m.printDocument();
    m.print();

    puts("undo");
    m.undo();
    m.printDocument();
    puts("undo");
    m.undo();
    m.printDocument();
    puts("undo");
    m.undo();
    m.printDocument();
    puts("undo");
    m.undo();
    m.printDocument();
    puts("undo");
    m.undo();
    m.printDocument();

    MiniDoc::MiniDoc<CharHolderEncoded, CharHolder> m2;

    printf("sizeof MiniDoc::MiniDoc<CharHolderEncoded, CharHolder>: %zu\n", sizeof(MiniDoc::MiniDoc<CharHolderEncoded, CharHolder>));
    printf("sizeof MiniDoc::MiniDoc<CharHolderEncoded, CharHolder>::Info: %zu\n", sizeof(MiniDoc::MiniDoc<CharHolderEncoded, CharHolder>::Info));

    auto conv = [](const CharHolderEncoded * in, int*outHex, char*outChar) { *outHex = (int)in->decode(); *outChar = (char)in->decode(); };

    m2.load(nullptr);
    m2.print(conv);
    m2.append(CharHolder::convert("hi").ptr());
    m2.print(conv);

    m.load("");
    // m.set_supports_advanced_undo(false);
    m.append("all\nwhere\noccupied");
    m.seek_line_start(1);
    m.insert(m.cursor(), "the\nseats\n");
    m.seek_line_start(2);
    m.insert(m.cursor(), "clean\n");
    m.seek_line_start(5);
    m.insert(m.cursor(), "duly\n");
    m.seek_line_start(3);
    m.insert(m.cursor(), "blue\n");
    m.seek_line_start(4);
    m.replace(m.cursor(), m.line_length()-1, "tables");
    m.print();

    puts("undo");
    m.undo();
    m.undo();
    m.undo();
    m.undo();
    m.undo();
    puts("append");
    m.append("apples");
    m.print();
    m.error("expected ';' at end of file");
    // m.set_supports_advanced_undo(true);

    m.load(nullptr);
    m.append("a\nb\nc\nd");
    m.print();
    m.seek_line(1);
    m.erase(m.cursor(), m.line_length());
    m.print();
    m.undo();
    m.print();
    m.seek_line(2);
    m.insert(m.cursor(), "cc");
    m.print();
    m.printDocument();
    m.seek_line(2);
    m.append("FOO\nBAR\n");
    m.print();
    m.printDocument();
    m.append("pie");
    m.print();
    m.printDocument();
    m.append("food\nloaf");
    m.print();
    m.printDocument();
    m.seek_line(0);
    m.error("line 0");
    m.seek_line(1);
    m.error("line 1");
    m.seek_line(2);
    m.error("line 2");
    m.seek_line(3);
    m.error("line 3");
    m.seek_line(4);
    m.error("line 4");
    m.seek_line(5);
    m.error("line 5");
    m.seek_line(6);
    m.error("line 6");
    m.seek_line(7);
    m.error("line 7");
    m.seek_line(8);
    m.error("line 8");

    return 0;
}
