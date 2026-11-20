// -ffast-contracts: the wrapper assumes only a postcondition that the
// callee enforces.  One the callee observes may be false after the call.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -ffast-contracts -O2" }
// { dg-additional-options {-fcontract-configuration=\[{"match":{"caller":false,"kind":"post"},"output":{"semantic":"observe"}}\]} }

int f (int x) pre (x > 0) post (r: r > 0);
void dead ();

int
use (int x)
{
  int r = f (x);
  if (r <= 0)
    dead ();
  return r;
}

// { dg-final { scan-assembler {_Z4deadv} } }
