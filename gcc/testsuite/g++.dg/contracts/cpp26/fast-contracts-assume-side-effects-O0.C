// Without optimization -ffast-contracts makes its assumptions as [[assume]]
// does, which never evaluates the predicate: each predicate is evaluated
// by its check alone.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -ffast-contracts -O0 -DPRE=1 -DPOST=1" }

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
