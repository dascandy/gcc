// -fcontract-postcheck-entry across translation units: callers that check
// the preconditions themselves call postcheck entry points defined in
// postcheck-entry-multi-tu-2.cc, for an ordinary function and a
// constructor, and use an inline function whose definition both
// translation units have.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -fcontracts-client-check=pre -fcontract-evaluation-semantic=enforce" }
// { dg-additional-sources "postcheck-entry-multi-tu-2.cc" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include "postcheck-entry-multi-tu.h"

int main ()
{
  npre = 0;
  if (defined_elsewhere (2) != 3 || npre != 1)
    __builtin_abort ();
  npre = 0;
  T t (4);
  if (t.t != 4 || npre != 1)
    __builtin_abort ();
  npre = 0;
  if (in_both (5) != 5 || npre != 1 || other_tu (5) != 5)
    __builtin_abort ();
}
