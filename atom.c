#include <stdlib.h>
#include "atom.h"

Atom *atom_make(int v) {
  Atom *a = (Atom *)malloc(sizeof(Atom));
  a->val = v;
  return a;
}
