#include <stdio.h>
#include <string.h>
#include "../src/lisp.h"
#include "test.h"
  
token table[] = {
  { TK_OPEN, "(" },
  { TK_SYMBOL, "hello" },
  { TK_OPEN, "(" },
  { TK_SYMBOL, "my" },
  { TK_SYMBOL, "name" },
  { TK_SYMBOL, "is" },
  { TK_SYMBOL, "wil" },
  { TK_CLOSE, ")" },
  { TK_CLOSE, ")" },
  { 0, 0 }
};

void simple_token_test() {

  token *t = token_make(TK_OPEN, "hello", strlen("hello"));
 
  check(strcmp(t->val, "hello") == 0, "correct val");
  check(t->typ == TK_OPEN, "correct typ");
  
  token_free(t);
}

void tokenize_test() {

  char msg[50]; 
  
  token *ts = tokenize("(hello (my name is wil))");
  for (token *t = table; t->val; t++) {
    sprintf(msg, "correct val: %s == %s", t->val, ts->val);
    check(strcmp(t->val, ts->val) == 0, msg);
    sprintf(msg, "correct typ: %d == %d", t->typ, ts->typ);
    check(t->typ == ts->typ, msg);
    ts = ts->next;
  }
}
