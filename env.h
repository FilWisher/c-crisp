typedef struct entry {
  const char *k;
  Atom *v;
  struct entry *next;
} Entry;

typedef struct env {
  int size;
  int capacity;
  struct env *parent;
  Entry **content;
} Env;

// create new empty env with parent p and initial capacity
Env *env_make(Env *p);
// store atom v under key k in env
void env_put(Env *env, const char *k, Atom *v);
// get atom stored under k in env, NULL otherwise
Atom *env_get(Env *env, const char *k);
