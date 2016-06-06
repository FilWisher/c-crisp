/* TODO: env must support: 
  - deriving new env 
  - binding names to atoms
  - looking up names in env
*/

typedef struct {} env;

atom *eval(env *e, atom *exp) {
  /*  TODO:
    if number or string
      return it
    if symbol
      look it up in env
    if list
      apply first arg to rest
  */

}


atom *apply(atom *op, atom *args) {
  /* TODO:
    create new env with names bound to args
    eval body in new env
  */
}
