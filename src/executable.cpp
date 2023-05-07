#include <minidoc.h>

int main() {
  printf("hi\n");
  
  MiniDoc::MiniDoc_T m;
  
  printf("sizeof MiniDoc: %zu\n", sizeof(MiniDoc::MiniDoc_T));
  printf("sizeof MiniDoc::Info: %zu\n", sizeof(MiniDoc::MiniDoc_T::Info));
 
  m.load("1234");
  m.print();
  m.replace(1, "47");
  m.print();
  m.replace(1, "86");
  m.print();
  m.replace(1, 3, "55");
  m.print();
  
  m.load("");
  m.print();
  m.append("A");
  m.print();
  puts("undo");
  m.undoStack().undo();
  m.print();
  puts("redo");
  m.undoStack().redo();
  m.print();
  
  m.load("");
  m.print();
  m.append("A");
  m.append("B");
  m.append("C");
  m.print();
  puts("undo");
  m.undoStack().undo();
  m.print();
  puts("undo");
  m.undoStack().undo();
  m.print();
  puts("undo");
  m.undoStack().undo();
  m.print();
  puts("append X");
  m.append("X");
  m.print();
  puts("undo");
  m.undoStack().undo();
  m.print();
  puts("undo");
  m.undoStack().undo();
  m.print();
  puts("undo");
  m.undoStack().undo();
  m.print();
  puts("undo");
  m.undoStack().undo();
  m.print();
  puts("undo");
  m.undoStack().undo();
  m.print();
  puts("undo");
  m.undoStack().undo();
  m.print();
  puts("undo");
  m.undoStack().undo();
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
  m.replace(m.cursor(), m.line_length(), "tables");
  m.printDocument();
  m.print();
  
  return 0;
}
