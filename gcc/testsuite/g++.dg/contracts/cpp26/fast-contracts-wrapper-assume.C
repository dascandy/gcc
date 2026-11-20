// -ffast-contracts: a caller-side wrapper that has checked f's precondition
// calls f.postcheck, which checks the postcondition, and then assumes the
// postcondition as [[assume (r > 0)]] would, so the test of r folds away.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -ffast-contracts -O2" }

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

// { dg-final { scan-assembler {_Z1fi\.postcheck} } }
// { dg-final { scan-assembler-not {_Z4deadv} } }
