#include <minidoc.h>

int main() {
  printf("hi\n");
  
  MiniDoc::MiniDoc m;
  
  printf("sizeof minidoc: %zu\n", sizeof(m));
 
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
  
  return 0;
}
