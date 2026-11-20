// Second translation unit of postcheck-entry-multi-tu.C.

#include "postcheck-entry-multi-tu.h"

int npre;
bool P (bool b) { ++npre; return b; }

int defined_elsewhere (const int x) { return x + 1; }

T::T (const int x) : t (x) {}

int other_tu (int x) { return in_both (x); }
