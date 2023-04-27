# MiniDoc

a mini document editor

includes both basic and full undo/redo capabilities

see minidoc.h for details

## basic usage

load a character stream `const char*` and an optional length `size_t`

for insert, replace, and erase operations, position 0 represents index 0, for zero based index, just like a C array

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