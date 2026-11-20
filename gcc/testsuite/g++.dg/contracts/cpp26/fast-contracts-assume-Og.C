// Under -Og, where __builtin_unreachable traps, -ffast-contracts makes its
// assumptions as [[assume]] does rather than evaluating the predicate, so
// they add no check: the wrapper's postcondition about memory is not
// used, and there is no trap.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -ffast-contracts -Og" }

void dead ();
struct S { int v; void set (int x) pre (x > 0) post (v > 0); };

int
use (S &s, int x)
{
  s.set (x);
  if (s.v <= 0)
    dead ();
  return s.v;
}

// { dg-final { scan-assembler {_Z4deadv} } }
// { dg-final { scan-assembler-not {\tud2} } }
