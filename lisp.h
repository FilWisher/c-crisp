// type tokens
enum type {
  T_PAIR,
  T_STRING,
  T_SYMBOL,
  T_NUMBER,
  T_NIL,
  T_ERROR
};

struct pair;

// main cell
typedef struct cell {
  enum type typ;
  char *val;
  union {
    struct pair *pair;
    int number;
  };
} Cell;

typedef struct pair {
  struct cell *car, *cdr;
} Pair;

typedef struct entry {
  const char *k;
  Cell *v;
  struct entry *next;
} Entry;

typedef struct env {
  int size;
  int capacity;
  struct env *parent;
  Entry **content;
} Env;

enum tok {  
  TK_OPEN,
  TK_CLOSE,
  TK_NIL,
  TK_SYMBOL
};

typedef struct token {
  enum tok typ;
  char *val;
  struct token *next, *prev;
} Token;

typedef struct {
  Token *start, *end;
} Stack;


// create new empty env with parent p and initial capacity
Env *env_make(Env *p);
// store atom v under key k in env
void env_put(Env *env, const char *k, Cell *v);
// get atom stored under k in env, NULL otherwise
Cell *env_get(Env *env, const char *k);

Cell *apply(Cell *fn, Cell *args);
Cell *eval(Cell *expr, Env *env);
void cell_print(Cell *c);
Cell *cell_read(FILE *f);
Cell *cell_make(enum type t, char *input);

Cell *parse(char *input);
Cell *parse_cell(Token *t);
Cell *parse_list(Stack *s, Token *t);
Stack *stack_make();

Token *token_make(enum tok typ, char *val);
Token *token_make_n(enum tok typ, char *val, int n);
void stack_push(Stack *s, Token *t);
Token *stack_pop(Stack *s);
void stack_print(Stack *s);
Stack *tokenize(char *input);


void dbg(char *msg);
