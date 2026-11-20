// -ffast-contracts evaluates the predicates of the assumptions it makes
// whatever their side effects: the caller-side wrapper's assumption of the
// postcondition and the precondition's at the postcheck entry point each
// evaluate their predicate once more than the checks do.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -ffast-contracts -O2 -DPRE=2 -DPOST=2" }

static int npre, npost;
static bool P (bool b) { ++npre; return b; }
static bool Q (bool b) { ++npost; return b; }

__attribute__ ((noipa)) int
f (int x) pre (P (x > 0)) post (r: Q (r > 0))
{
  return x;
}

int
main ()
{
  f (1);
  if (npre != PRE || npost != POST)
    __builtin_abort ();
}
