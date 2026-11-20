// Without -ffast-contracts the caller-side wrapper does not assume f's
// postcondition, so the test of r stays.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -fcontracts-client-check=pre -O2" }

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
// { dg-final { scan-assembler {_Z4deadv} } }
