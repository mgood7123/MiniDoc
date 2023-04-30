#include <minidoc.h>

int main() {
  printf("hi\n");
  
  MiniDoc::MiniDoc<char> m;
  
  printf("sizeof MiniDoc: %zu\n", sizeof(MiniDoc::MiniDoc<char>));
  printf("sizeof MiniDoc::Info: %zu\n", sizeof(MiniDoc::MiniDoc<char>::Info));
 
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
  
  
  return 0;
}
