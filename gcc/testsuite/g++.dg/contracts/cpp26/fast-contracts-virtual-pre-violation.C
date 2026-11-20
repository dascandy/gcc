// -ffast-contracts with P3097: where D::f narrows B::f's preconditions, a
// call through B::f that satisfies B::f's but not D::f's is still
// reported by D::f.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
#include <cstdlib>
#include <cstring>

#define EXPECTED "x > 10"

void
handle_contract_violation (const std::contracts::contract_violation &v)
{
  if (std::strcmp (v.comment (), EXPECTED) == 0)
    std::exit (0);
  std::abort ();
}

struct B { virtual int f (int x) pre (x > 0); };
struct D : B { int f (int x) override pre (x > 10); };
int B::f (int x) { return x + 10; }
int D::f (int x) { return x; }

__attribute__ ((noipa)) int call (B *p, int x) { return p->f (x); }

int
main ()
{
  D d;
  call (&d, 5);
  return 1;
}
