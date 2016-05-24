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

enum a_typ { A_PAIR, A_STRING, A_SYMBOL, A_NUMBER, A_ERROR, A_NIL };

// forward declaration
struct atom;

struct pair {
  struct atom *car, *cdr;
};

typedef struct atom {
  enum a_typ typ;
  char *val;
  union {
    int number;
    struct pair pair;
  };
} atom;

atom *atom_make(enum a_typ typ, char *val);
void atom_free(atom *a);
atom *parse(char *input);
