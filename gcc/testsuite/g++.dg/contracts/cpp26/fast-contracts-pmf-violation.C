// -ffast-contracts with P3097: a call through a pointer to member function
// made from a virtual function checks that function's preconditions, as
// a virtual call does, before dispatching to the final overrider, which
// assumes them.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
#include <cstdlib>
#include <cstring>

void
handle_contract_violation (const std::contracts::contract_violation &v)
{
  if (std::strcmp (v.comment (), "x > 0") == 0)
    std::exit (0);
  std::abort ();
}

struct B { virtual int f (int x) pre (x > 0); };
struct D : B { int f (int x) override pre (x > -5); };
int B::f (int x) { return x; }
int D::f (int x) { return x; }

__attribute__ ((noipa)) int
call (B *b, int (B::*pmf) (int), int x)
{
  return (b->*pmf) (x);
}

int
main ()
{
  D d;
  call (&d, &B::f, -1);
  return 1;
}
