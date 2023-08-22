# MiniDoc

a mini document editor

supports `basic` and `advanced` undo/redo

built-in `patch dependency system` powered by `Darcs` (Haskell Darcs)

see `minidoc.h` for details

## basic usage

first we require a two template paramaters

1. `T` the type to use for data, `char_t`
2. a suitible string adapter, `adapter_t`

`MiniDoc_T` is provided as a common case for `MiniDoc<char, StringAdapter::CharAdapter>`

next, load a character stream `const T*` and an optional length `size_t`, the input `will be copied`

use `sub_str` to copy a range of the output, best used for copying output in chunks, best used for large documents 

use `str` to copy the entire output, best used for small documents

use `line` to copy the specified line of the output, best used for line-by-line output

use `seek` and `character` to obtain the character at the specified position

for `insert`, `replace`, and `erase` operations, position 0 represents index 0, uses zero based index, just like a C array

the position and length are clamped to the bounds of the document (0 to length) in respect to all current modifications done to the document

`append` appends to the `end` of the document

```
doc: apple
append("ban")
doc: appleban
```

`insert` appends to the specified position, use `-1` to specify end of document

```
doc: apple
insert(-1, "ban")
doc: appleban

doc: apple
insert(1, "ban")
doc: abanpple
```

`replace` replaces the text at the specified `position`, with an optional `length` (default `length` is `1`), with the given text

```
doc: apple
replace(1, "ban")
doc: abanple
```

if given `-1` for `position`, the behaviour is the same as if `append`, and `length` is ignored

if given `-1` for `length`, length will be truncated to the remainder based on `position`

`erase` erases the text at the specified `position`, with a given `length`

```
doc: apple
erase(1, 3)
doc: ae
```

```
doc: apple
erase(1, -1)
doc: a
```

if given `-1` for `position`, nothing is done

if given `-1` for `length`, erases all text at and after `position`

`seek`, `seek_line*`, `next`, `previous`, `has_next`, `has_previous`, and `cursor` can be used for manipulating and monitoring the text cursor inside the document

`seek*` is clamped to the bounds of the document (0 to length/line) in respect to all current modifications done to the document

pass `cursor` as a `position` or a `length`  to implement various capabilities such as deleting text at the cursor (`void backspace() { auto c = cursor(); if (c != 0) erase(c-1, c); }`) and others

`print` prints detailed information about the current state

```cpp
tag: 
lines: 1
length: 1
cursor: 0
line start: 0
line end: 2
line length: 2
line: 0
column: 0
line str: a
line str size: 1
line str hex
        0x000000: 61                       a

character:  'a'
Undo Stack: 1 items in undo stack
    undo #0 : Minidoc Command: content: "pple", content2: ""
Undo Stack: 0 items in redo stack

```

### Backtrace/Error

basic error reporting is provided via `error()` and `error(const std::string & message)` functions

an error will trigger a `backtrace` to be collected at the current document `line` as reported by `line()`, starting with the `most recent edit` to occur `on that line`

after the `backtrace` has been collected it will then be printed

```cpp
    m.append("apples");
    m.print();
    m.error("expected ';' at end of file");
```

```cpp
tag: 
lines: 3
length: 24
cursor: 18
line start: 10
line end: 25
line length: 15
line: 2
column: 8
line str: occupiedapples
line str size: 14
line str hex
        0x000000: 6f 63 63 75 70 69 65 64  occupied
        0x000008: 61 70 70 6c 65 73        apples

character:  'a'
Undo Stack: 12 items in undo stack
    undo #0 : Minidoc Command: content: "all\nwhere\noccupied", content2: "": { line 0, old lines 1, new lines 3 }
    undo #1 : Minidoc Command: content: "the\nseats\n", content2: "", hunk: { line 1, old lines 1, new lines 3 }
    undo #2 : Minidoc Command: content: "clean\n", content2: "", hunk: { line 2, old lines 1, new lines 2 }
    undo #3 : Minidoc Command: content: "duly\n", content2: "", hunk: { line 5, old lines 1, new lines 2 }
    undo #4 : Minidoc Command: content: "blue\n", content2: "", hunk: { line 3, old lines 1, new lines 2 }
    undo #5 : Minidoc Command: content: "seats", content2: "tables", hunk: { line 4, old lines 1, new lines 1 }
    undo #6 : InvertCommand: Minidoc Command: content: "seats", content2: "tables", hunk: { line 4, old lines 1, new lines 1 }
    undo #7 : InvertCommand: Minidoc Command: content: "blue\n", content2: "", hunk: { line 3, old lines 2, new lines 1 }
    undo #8 : InvertCommand: Minidoc Command: content: "duly\n", content2: "", hunk: { line 5, old lines 2, new lines 1 }
    undo #9 : InvertCommand: Minidoc Command: content: "clean\n", content2: "", hunk: { line 2, old lines 2, new lines 1 }
    undo #10 : InvertCommand: Minidoc Command: content: "the\nseats\n", content2: "", hunk: { line 1, old lines 3, new lines 1 }
    undo #11 : Minidoc Command: content: "apples", content2: "", hunk: { line 2, old lines 1, new lines 1 }
Undo Stack: 0 items in redo stack


expected ';' at end of file
      line 1, column 0 : "where\n"
   at line 2, column 8 : "occupiedapples"
                                  ^
printing backtrace...
 edit #12: before insert: "apples"
      line 1, column 0 : "where\n"
   at line 2, column 8 : "occupied"
                                  ^
 edit #9: before erase: "duly\n"
      line 2, column 0 : "clean\n"
   at line 3, column 4 : "seats\n"
                              ^
      line 4, column 0 : "where\n"

 edit #4: before insert: "duly\n"
      line 4, column 0 : "where\n"
   at line 5, column 0 : "occupied"
                          ^
 edit #1: before insert: "all\nwhere\noccupied"
   at line 0, column 0 : ""
                          ^
 edit #0: ADD FILE
end of backtrace
```

### Undo Stack <T>

we support both `basic` and `advanced` undo

set `supports_redo` to specify if `redo` should be tracked

set `supports_advanced_undo` to toggle between `basic` and `advanced` undo

`state` information is automatically saved upon every `edit` operation

use `undo` and `redo` to iterate between saved `states`

the undo stack is displayed via `print`

```cpp
Undo Stack: 7 items in undo stack
    undo #0 : Minidoc Command: content: "A", content2: ""
    undo #1 : Minidoc Command: content: "B", content2: ""
    undo #2 : Minidoc Command: content: "C", content2: ""
    undo #3 : InvertCommand: Minidoc Command: content: "C", content2: ""
    undo #4 : InvertCommand: Minidoc Command: content: "B", content2: ""
    undo #5 : InvertCommand: Minidoc Command: content: "A", content2: ""
    undo #6 : Minidoc Command: content: "X", content2: ""
Undo Stack: 0 items in redo stack
```

#### Basic
in `basic` mode, we mimic most/all modern editors:

any edit `erases` the `redo` stack

all `redo states` are `lost` if you `undo` then `edit`

#### Advanced
in `advanced` mode, we go an extra step to `fully preserve` the `undo/redo` stack upon `edit`

any edit `preserves` the `redo` stack

all `redo states` are `preserved` if you `undo` then `edit`

#### example

```cpp
  m.load("");
  m.append("A");
  // the current undo stack is as follows
  // ""
  // the current redo stack is as follows
  //
  m.append("B");
  // the current undo stack is as follows
  // "A" > ""
  // the current redo stack is as follows
  //
  m.append("C");
  // the current undo stack is as follows
  // "AB" > "A" > ""
  // the current redo stack is as follows
  //
  m.undoStack().undo();
  // the current undo stack is as follows
  // "AB" > "A" > ""
  // the current redo stack is as follows
  // "ABC"
  m.undoStack().undo();
  // the current undo stack is as follows
  // "A" > ""
  // the current redo stack is as follows
  // "AB" > "ABC"
  m.undoStack().undo();
  // the current undo stack is as follows
  // ""
  // the current redo stack is as follows
  // "A" > "AB" > "ABC"
  
  // we are now at empty input
  
  m.append("X");
  
  // in basic, an edit would not be able to recover "A"
  // due to the redo stack that contains A, AB, and ABC
  // being erased
  //
  // the current undo stack is as follows
  // ""
  // the current redo stack is as follows
  //
  
  // in advance, an edit would preserve this information
  //
  // the current undo stack is as follows
  // "" > "A" > "AB" > "ABC" > "AB" > "A" > ""
  // the current redo stack is as follows
  //
  
  m.undoStack().undo();
  // the current undo stack is as follows BASIC
  // ""
  // the current redo stack is as follows BASIC
  // "X"
  //
  // the current undo stack is as follows ADVANCED
  // "" > "A" > "AB" > "ABC" > "AB" > "A" > ""
  // the current redo stack is as follows ADVANCED
  // "X"
```