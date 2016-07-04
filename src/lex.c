#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lisp.h"

token *token_make(enum tk_typ typ, char *val, int len) {
  token *t = (token *)malloc(sizeof(t));
  t->typ = typ;
  t->val = strndup(val, len);
  return t;
}

void token_free(token *t) {
  free(t->val);
  free(t);
}

int issymbol(char c) {
  return (isalnum(c) || (ispunct(c) && c != '(' && c !=')'));
}

int isstring(char c) {
  return (c != '"' && c != ' ');
}

token *tokenize(char *text) {
  
  char *tp = text;
  token *tok = NULL, *head = NULL, *tail = NULL;
  while (*tp != '\0') {
    int len = 0;
    while (isspace(*tp)) tp++;
    switch (*tp) {
    case '(':
      tok = token_make(TK_OPEN, "(", 1);
      tp += 1;
      break;
    case ')':
      tok = token_make(TK_CLOSE, ")", 1);
      tp += 1;
      break;
    case '"':
      tp++;
      while (isstring(*(tp+len))) len++;
      tok = token_make(TK_STRING, tp, len);
      break;
    default:
      while (issymbol(*(tp+len))) len++;
      tok = token_make(TK_SYMBOL, tp, len);
      tp += len;
    }
    if (!head) head = tok;
    
    if (!tail) tail = tok; 
    else {
      tail->next = tok;
      tail = tail->next;
    }
  }
  return head;
}
