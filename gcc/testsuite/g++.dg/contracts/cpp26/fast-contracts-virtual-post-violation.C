// -ffast-contracts with P3097: D::f, which overrides B::f, checks B::f's
// postconditions on every return from a call whose arguments satisfy
// B::f's preconditions -- also a call that names D::f itself.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
#include <cstdlib>
#include <cstring>

#define EXPECTED "r > 5"

void
handle_contract_violation (const std::contracts::contract_violation &v)
{
  if (std::strcmp (v.comment (), EXPECTED) == 0)
    std::exit (0);
  std::abort ();
}

struct B { virtual int f (int x) pre (x > 0) post (r: r > 5); };
struct D : B { int f (int x) override pre (x > -5) post (r: r > 0); };
int B::f (int x) { return x + 10; }
int D::f (int x) { return x; }

int
main ()
{
  D d;
  volatile int x = 3;
  d.f (x);
  return 1;
}
