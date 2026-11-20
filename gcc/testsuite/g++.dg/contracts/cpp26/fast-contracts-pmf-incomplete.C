// -ffast-contracts with P3097: a pointer to member function made while its
// class is incomplete, from a function not yet known to be virtual (it
// overrides B::f without saying so), and used only in a variable's
// initializer, which is lowered after the contract wrappers are emitted,
// still has its thunk defined -- and checks the precondition through it.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
#include <cstdlib>
#include <cstring>

void
handle_contract_violation (const std::contracts::contract_violation &v)
{
  if (std::strcmp (v.comment (), "x > 1") == 0)
    std::exit (0);
  std::abort ();
}

struct B { virtual int f (int x) pre (x > 0); };
struct D : B
{
  int f (int x) pre (x > 1);
  static constexpr int (D::*p) (int) = &D::f;
};
int B::f (int x) { return x; }
int D::f (int x) { return x + 1; }

int (D::*glob) (int) = D::p;

__attribute__ ((noipa)) int
call (D *d, int x)
{
  return (d->*glob) (x);
}

int
main ()
{
  D d;
  if (call (&d, 2) != 3)
    std::abort ();
  call (&d, 1);
  return 1;
}
