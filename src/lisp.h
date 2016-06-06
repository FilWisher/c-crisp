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

enum a_typ { A_PAIR, A_STRING, A_SYMBOL, A_NUMBER, A_ERROR, A_NIL, A_ENV, A_FN };

// forward declaration
struct atom;
struct env;

struct pair {
  struct atom *car, *cdr;
};

#define car(a) (a->pair.car)
#define cdr(a) (a->pair.cdr)

typedef struct atom {
  enum a_typ typ;
  char *val;
  union {
    int number;
    struct pair pair;
    struct env *env;
    struct atom *(*fn)(struct env *e, struct atom *args);
  };
} atom;

typedef struct {
  token *tks, *tp;
} parser;

atom *atom_make(enum a_typ typ, char *val);
void atom_free(atom *a);
atom *parse(char *input);
atom *parse_switch(parser *p);

typedef struct env_entry {
  char *k;
  atom *v;
  struct env_entry *next;
} env_entry;

typedef struct env {
  int n;
  int cap;
  struct env *parent;
  env_entry **table;
} env;

int equal(char *a, char *b);

env *env_make(env *parent, int cap);
atom *make_fn(env *e, atom *params, atom *body);
void *env_bind(env *e, char *k, atom *v);
atom *env_lookup(env *e, char *k);
unsigned int hash(char *k); 

atom *eval(env *e, atom *exp);
atom *apply(atom *op, atom *args);
int atom_len(atom *a);
void atom_print(atom *a);

void load_core(env *e);
