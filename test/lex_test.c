#include <stdio.h>
#include <string.h>
#include "../src/lisp.h"
#include "test.h"

void simple_token_test() {

  token *t = token_make(TK_OPEN, "hello", strlen("hello"));
 
  check(strcmp(t->val, "hello") == 0, "correct val");
  check(t->typ == TK_OPEN, "correct typ");
  
  token_free(t);
}

void tokenize_test() {

  char msg[50]; 
  token *ts = tokenize("(hello (my name is wil))");
  token table[] = {
    { TK_OPEN, "(" },
    { TK_SYMBOL, "hello" },
    { TK_OPEN, "(" },
    { TK_SYMBOL, "my" },
    { TK_SYMBOL, "name" },
    { TK_SYMBOL, "is" },
    { TK_SYMBOL, "wil" },
    { TK_CLOSE, ")" },
    { TK_CLOSE, ")" }
  };
  token *tp = ts;
  for (int i = 0; i < sizeof(table)/sizeof(token); i++) {
    sprintf(msg, "correct val: %s == %s", (table+i)->val, tp->val);
    check(strcmp((table+i)->val, tp->val) == 0, msg);
    sprintf(msg, "correct typ: %d == %d", (table+i)->typ, tp->typ);
    check((table+i)->typ == tp->typ, msg);
    tp = tp->next;
  }
}
