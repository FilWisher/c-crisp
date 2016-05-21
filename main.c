#include <stdio.h>
#include <stdlib.h>
#include "lisp.h"

void check(Env *e) {
  static int n = 0;
  if (e->parent == NULL) printf ("%d: parent null here\n", n++);
  else printf("%d: parent NOT null here\n", n++);
}

int main() {

  Cell *c;
  Env *e = env_make(NULL); //global
  
  Cell *test = cell_make(T_NUMBER, "22");
  test->number = 22;
  env_put(e, "cool", test);

  for (;;) {
    
    fputs("\n> ", stdout);
    dbg("main");
    c = cell_read(stdin);
    dbg("after read");
    cell_print(eval(c, e));
    dbg("after print");
    free(c);
    dbg("after free");
  }
}
