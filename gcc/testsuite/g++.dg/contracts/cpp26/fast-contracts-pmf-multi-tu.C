// -ffast-contracts with P3097: pointers to member functions made from the
// same virtual function in different translation units compare equal,
// since they point to the same thunk.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }
// { dg-additional-sources "fast-contracts-pmf-multi-tu-2.cc" }

#include "fast-contracts-pmf-multi-tu.h"

int
main ()
{
  if (other_tu () != &B::f)
    __builtin_abort ();
}
