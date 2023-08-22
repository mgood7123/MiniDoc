#include <gtest/gtest.h>

#define MINIDOC_GENERIC_PIECE_TABLE_FUNCTION_TYPE DarcsPatch::function
#define STRING_ADAPTER_FUNCTION_TYPE DarcsPatch::function

#include <minidoc.h>

#define match_info(info_, lines_, length_, character_, cursor_, line_, column_, line_start_, line_end_, line_length_) \
    { \
        auto & info = info_; \
        ASSERT_EQ(info.lines(), lines_); \
        ASSERT_EQ(info.length(), length_); \
        ASSERT_EQ(info.cursor(), cursor_); \
        ASSERT_EQ(info.line(), line_); \
        ASSERT_EQ(info.column(), column_); \
        ASSERT_EQ(info.line_start(), line_start_); \
        ASSERT_EQ(info.line_end(), line_end_); \
        ASSERT_EQ(info.line_length(), line_length_); \
        ASSERT_EQ(info.character(), character_); \
    }

TEST(MiniDoc, init) {
    MiniDoc::MiniDoc_T document;
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 1,     0,      '\0',      0,      0,    0,      0,          1,        1)
    ASSERT_EQ(document.get_info().str().c_str().length(), 0);
    ASSERT_STREQ(document.get_info().str().c_str().ptr(), "");
    ASSERT_STREQ(document.get_info().str().c_str().ptr(), "");
}

TEST(MiniDoc, load) {
    MiniDoc::MiniDoc_T document;
    document.load("1");
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 1,     1,      '1',       0,      0,    0,      0,          2,        2)
    ASSERT_EQ(document.get_info().str().c_str().length(), 1);
    ASSERT_STREQ(document.get_info().str().c_str().ptr(), "1");
    ASSERT_STREQ(document.get_info().str().c_str().ptr(), "1");
}

TEST(MiniDoc, next_previous) {
    MiniDoc::MiniDoc_T document;
    document.load("a\nb\n\n\nc\nd");
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'a',       0,      0,    0,      0,          2,        2)
    ASSERT_EQ(document.has_next(), true);
    document.next();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      1,      0,    1,      0,          2,        2)
    ASSERT_EQ(document.has_next(), true);
    document.next();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'b',       2,      1,    0,      2,          4,        2)
    ASSERT_EQ(document.has_next(), true);
    document.next();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      3,      1,    1,      2,          4,        2)
    ASSERT_EQ(document.has_next(), true);
    document.next();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      4,      2,    0,      4,          5,        1)
    ASSERT_EQ(document.has_next(), true);
    document.next();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      5,      3,    0,      5,          6,        1)
    ASSERT_EQ(document.has_next(), true);
    document.next();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'c',       6,      4,    0,      6,          8,        2)
    ASSERT_EQ(document.has_next(), true);
    document.next();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      7,      4,    1,      6,          8,        2)
    ASSERT_EQ(document.has_next(), true);
    document.next();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'd',       8,      5,    0,      8,          10,       2)
    ASSERT_EQ(document.has_next(), true);
    document.next();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\0',      9,      5,    1,      8,          10,       2)
    ASSERT_EQ(document.has_next(), false);
    document.next();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\0',      9,      5,    1,      8,          10,       2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), false);
    document.seek(0);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'a',       0,      0,    0,      0,          2,        2)
    ASSERT_EQ(document.has_next(), true);
    document.seek(-1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\0',      9,      5,    1,      8,          10,       2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), false);

    document.seek(0);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'a',       0,      0,    0,      0,          2,        2)
    ASSERT_EQ(document.has_next(), true);
    document.seek_line(0);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'a',       0,      0,    0,      0,          2,        2)
    ASSERT_EQ(document.has_next(), true);
    document.seek_line(1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'b',       2,      1,    0,      2,          4,        2)
    ASSERT_EQ(document.has_next(), true);
    document.seek(-1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\0',      9,      5,    1,      8,          10,       2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), false);
    document.seek_line(1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'b',       2,      1,    0,      2,          4,        2)

    document.seek_line(0);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'a',       0,      0,    0,      0,          2,        2)
    ASSERT_EQ(document.has_next(), true);
    document.seek_line_start(1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'b',       2,      1,    0,      2,          4,        2)
    ASSERT_EQ(document.has_next(), true);
    document.seek(-1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\0',      9,      5,    1,      8,          10,       2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), false);
    document.seek_line_start(1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'b',       2,      1,    0,      2,          4,        2)

    document.seek_line(0);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'a',       0,      0,    0,      0,          2,        2)
    ASSERT_EQ(document.has_next(), true);
    document.seek_line_start(1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'b',       2,      1,    0,      2,          4,        2)
    document.seek_line_end(1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      3,      1,    1,      2,          4,        2)
    ASSERT_EQ(document.has_next(), true);
    document.seek_line(0);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'a',       0,      0,    0,      0,          2,        2)
    ASSERT_EQ(document.has_next(), true);
    document.seek_line_end(1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      3,      1,    1,      2,          4,        2)
    ASSERT_EQ(document.has_next(), true);
    document.seek(-1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\0',      9,      5,    1,      8,          10,       2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), false);
    document.seek_line_end(1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      3,      1,    1,      2,          4,        2)

    document.seek(-1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\0',      9,      5,    1,      8,          10,       2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), false);
    document.previous();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'd',       8,      5,    0,      8,          10,       2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), true);
    document.previous();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      7,      4,    1,      6,          8,        2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), true);
    document.previous();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'c',       6,      4,    0,      6,          8,        2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), true);
    document.previous();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      5,      3,    0,      5,          6,        1)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), true);
    document.previous();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      4,      2,    0,      4,          5,        1)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), true);
    document.previous();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      3,      1,    1,      2,          4,        2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), true);
    document.previous();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'b',       2,      1,    0,      2,          4,        2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), true);
    document.previous();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      1,      0,    1,      0,          2,        2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), true);
    document.previous();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'a',       0,      0,    0,      0,          2,        2)
    ASSERT_EQ(document.has_previous(), false);
    ASSERT_EQ(document.has_next(), true);
    document.previous();
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'a',       0,      0,    0,      0,          2,        2)
    ASSERT_EQ(document.has_previous(), false);
    ASSERT_EQ(document.has_next(), true);
}

TEST(MiniDoc, seek_1) {
    MiniDoc::MiniDoc_T document;
    document.load("1");
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 1,     1,      '1',       0,      0,    0,      0,          2,        2)
    document.seek(1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 1,     1,      '\0',      1,      0,    1,      0,          2,        2)
    document.erase(1, 1);
    match_info(document.get_info(), 1,     1,      '\0',      1,      0,    1,      0,          2,        2)
}

TEST(MiniDoc, seek_2) {
    MiniDoc::MiniDoc_T document;
    document.load("a\nb\n\n\nc\nd");
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'a',       0,      0,    0,      0,          2,        2)
    ASSERT_EQ(document.has_previous(), false);
    ASSERT_EQ(document.has_next(), true);
    document.seek(1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      1,      0,    1,      0,          2,        2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), true);
    document.seek(0);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      'a',       0,      0,    0,      0,          2,        2)
    ASSERT_EQ(document.has_previous(), false);
    ASSERT_EQ(document.has_next(), true);
    document.seek(5);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      5,      3,    0,      5,          6,        1)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), true);
    document.seek(1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(document.get_info(), 6,     9,      '\n',      1,      0,    1,      0,          2,        2)
    ASSERT_EQ(document.has_previous(), true);
    ASSERT_EQ(document.has_next(), true);
}

TEST(MiniDoc, print) {
    MiniDoc::MiniDoc_T document;
    document.load("1");
    document.append("123");
    document.print();
}

TEST(MiniDoc, insert) {
    MiniDoc::MiniDoc_T m;
    m.load("");
    m.append("all\nwhere\noccupied");
    m.seek_line_start(1);
    m.insert(m.cursor(), "the\nseats\n");
    m.seek_line_start(2);
    m.insert(m.cursor(), "clean\n");
    m.seek_line_start(5);
    m.insert(m.cursor(), "duly\n");
    m.seek_line_start(3);
    m.insert(m.cursor(), "blue\n");
    ASSERT_STREQ(m.str().c_str().ptr(), "all\nthe\nclean\nblue\nseats\nwhere\nduly\noccupied");
}

TEST(MiniDoc, replace) {
    MiniDoc::MiniDoc_T m;
    m.load("");
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
    ASSERT_STREQ(m.str().c_str().ptr(), "all\nthe\nclean\nblue\ntables\nwhere\nduly\noccupied");
    m.load("1234");
    ASSERT_STREQ(m.str().c_str().ptr(), "1234");
    m.replace(1, "47"); // 1234 -> 14734
    ASSERT_STREQ(m.str().c_str().ptr(), "14734");
    m.replace(1, "86"); // 14734 -> 186734
    ASSERT_STREQ(m.str().c_str().ptr(), "186734");
    m.replace(1, 3, "55"); // 186734 -> 15534
    ASSERT_STREQ(m.str().c_str().ptr(), "15534");

    m.load("apple");
    m.append("ban");
    ASSERT_STREQ(m.str().c_str().ptr(), "appleban");

    m.load("apple");
    m.insert(-1, "ban");
    ASSERT_STREQ(m.str().c_str().ptr(), "appleban");

    m.load("apple");
    m.insert(1, "ban");
    ASSERT_STREQ(m.str().c_str().ptr(), "abanpple");

    m.load("apple");
    m.replace(0, -1, "ban");
    ASSERT_STREQ(m.str().c_str().ptr(), "ban");

    m.load("apple");
    m.replace(1, "ban");
    ASSERT_STREQ(m.str().c_str().ptr(), "abanple");

    m.load("apple");
    m.replace(1, 2, "ban");
    ASSERT_STREQ(m.str().c_str().ptr(), "abanle");

    m.load("apple");
    m.erase(1, 3);
    ASSERT_STREQ(m.str().c_str().ptr(), "ae");

    m.load("apple");
    m.erase(1, -1);
    ASSERT_STREQ(m.str().c_str().ptr(), "a");
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(m.get_info(),        1,     1,      'a',       0,      0,    0,      0,          2,        2)
    m.seek(0);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(m.get_info(),        1,     1,      'a',       0,      0,    0,      0,          2,        2)
    m.seek(1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(m.get_info(),        1,     1,      '\0',      1,      0,    1,      0,          2,        2)
    m.seek(-1);
    //         info                 lines  length  character  cursor  line  column  line start  line end  line length
    match_info(m.get_info(),        1,     1,      '\0',      1,      0,    1,      0,          2,        2)
}

TEST(MiniDoc, undo_redo_1) {
    MiniDoc::MiniDoc_T m;
    m.load("");
    ASSERT_STREQ(m.str().c_str().ptr(), "");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
    m.append("A");
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.append("B");
    ASSERT_STREQ(m.str().c_str().ptr(), "AB");
    m.append("C");
    ASSERT_STREQ(m.str().c_str().ptr(), "ABC");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "AB");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
    m.append("X");
    ASSERT_STREQ(m.str().c_str().ptr(), "X");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "AB");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "ABC");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "AB");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "AB");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "ABC");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "AB");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "X");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "X");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "X");
}

TEST(MiniDoc, undo_redo_2) {
    MiniDoc::MiniDoc_T m;
    m.load("");
    m.append("A");
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
    m.append("B");
    ASSERT_STREQ(m.str().c_str().ptr(), "B");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
}

TEST(MiniDoc, undo_redo_3) {
    MiniDoc::MiniDoc_T m;
    m.load("");
    m.append("A");
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.append("B");
    ASSERT_STREQ(m.str().c_str().ptr(), "AB");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.append("A");
    ASSERT_STREQ(m.str().c_str().ptr(), "AA");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "AB");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
}

TEST(MiniDoc, undo_redo_4) {
    MiniDoc::MiniDoc_T m;
    m.load("");
    m.append("A");
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.append("B");
    ASSERT_STREQ(m.str().c_str().ptr(), "AB");
    m.append("C");
    ASSERT_STREQ(m.str().c_str().ptr(), "ABC");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "AB");
    m.append("D");
    ASSERT_STREQ(m.str().c_str().ptr(), "ABD");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "AB");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "ABC");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "AB");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
}

TEST(MiniDoc, undo_redo__insert) {
    MiniDoc::MiniDoc_T m;
    m.load("");
    m.append("A");
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
}

TEST(MiniDoc, undo_redo__erase_1) {
    MiniDoc::MiniDoc_T m;
    m.load("");
    m.append("A");
    m.erase(0, 1);
    ASSERT_STREQ(m.str().c_str().ptr(), "");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "");
}

TEST(MiniDoc, undo_redo__erase_2) {
    MiniDoc::MiniDoc_T m;
    m.load("apple");
    m.erase(1, 3);
    ASSERT_STREQ(m.str().c_str().ptr(), "ae");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "apple");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "ae");
}

TEST(MiniDoc, undo_redo__erase_3) {
    MiniDoc::MiniDoc_T m;
    m.load("apple");
    m.erase(1, -1);
    ASSERT_STREQ(m.str().c_str().ptr(), "a");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "apple");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "a");
}

TEST(MiniDoc, undo_redo__replace_1) {
    MiniDoc::MiniDoc_T m;
    m.load("");
    m.append("A");
    m.replace(0, 1, "B");
    ASSERT_STREQ(m.str().c_str().ptr(), "B");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "B");
}

TEST(MiniDoc, undo_redo__replace_2) {
    MiniDoc::MiniDoc_T m;
    m.load("");
    m.append("A");
    m.replace(0, 1, "Bwrh");
    ASSERT_STREQ(m.str().c_str().ptr(), "Bwrh");
    m.undo();
    ASSERT_STREQ(m.str().c_str().ptr(), "A");
    m.redo();
    ASSERT_STREQ(m.str().c_str().ptr(), "Bwrh");
}
