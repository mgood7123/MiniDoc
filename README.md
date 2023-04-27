# MiniDoc

a mini document editor

includes both basic and full undo/redo capabilities

see minidoc.h for details

## basic usage

load a character stream `const char*` and an optional length `size_t`, the input `is not copied`

all modifications done to the document are reflected in the obtained output

use `sub_str` to copy a range of the output, best used for copying output in chunks, best used for large documents 

use `str` to copy the entire output, best used for small documents

use `line` to copy the specified line of the output, best used for line-by-line output

use `seek` and `character` to obtain the character at the specified position

for `insert`, `replace`, and `erase` operations, position 0 represents index 0, for zero based index, just like a C array

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

`seek`, `next`, `previous`, `has_next`, `has_previous`, and `cursor` can be used for manipulating and monitoring the text cursor inside the document

`seek` is clamped to the bounds of the document (0 to length) in respect to all current modifications done to the document

pass `cursor` as a `position` or a `length`  to implement various capabilities such as deleting text at the cursor (`void backspace() { auto c = cursor(); if (c != 0) erase(c-1, c); }`) and others

`print` prints detailed information about the current state

```
length: 1
cursor: 0
line start: 0
line end: 1
line length: 1
line: 0
column: 0
line str: a
line str size: 1
line str len: 1
line str hex
        0x000000: 61                       a

character:  'a'
undo stack: 1 items

  undo #0
    length: 5
    cursor: 0
    line start: 0
    line end: 5
    line length: 5
    line: 0
    column: 0
    line str: apple
    line str size: 5
    line str len: 5
    line str hex
        0x000000: 61 70 70 6c 65           apple

    character:  'a'


redo stack: 0 items
```

### Undo Stack <T>

we support both `basic` and `advanced undo

obtained via `undoStack`, an `UndoStackHolder` ensures the stack itself is not assigned, similar to a `get-only` property

set `supports_redo` to specify if `redo` should be tracked

set `supports_advanced_undo` to toggle between `basic` and `advanced` undo

use `push_undo` to push an `undo` state before modification

use `undo` and `redo` to iterate between these `states`

#### Basic
in `basic` mode, we mimic most/all modern editors:

any edit `erases` the `redo` stack

all `redo states` are `lost` if you `undo` then `push_undo`

#### Advanced
in `advanced` mode, we fully preserve the `undo/redo` stack upon edit

any edit `preserves` the `redo` stack

all `redo states` are `preserved` if you `undo` then `push_undo`

#### example

```
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