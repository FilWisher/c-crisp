enum tk_typ { TK_OPEN, TK_CLOSE, TK_SYMBOL };

typedef struct token {
  enum tk_typ typ;
  char *val;
  struct token *next; 
} token;

// create new token. mallocs string argument
token *token_make(enum tk_typ typ, char *val, int len);
void token_free(token *t);
token *tokenize(char *text);

enum a_typ { A_PAIR, A_STRING, A_SYMBOL, A_NUMBER };

typedef struct {
  enum a_typ typ;
  char *val;
  union {
    int number; 
  };
} atom;

atom *atom_make(enum a_typ typ, char *val);
void atom_free(atom *a);
atom *parse();
