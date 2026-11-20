// -ffast-contracts: calls through caller-side wrappers that assume the
// callee's postconditions behave as without the assumptions, at run time
// and in constant evaluation.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -ffast-contracts -O2" }

constexpr int h (int x) pre (x > 0) post (r: r > 1) { return x + 1; }
static_assert (h (1) == 2);

struct S
{
  int v;
  S (const int v) pre (v > 0) post (this->v == v) : v (v) {}
  int get (const int d) const pre (d >= 0) post (r: r == v + d) { return v + d; }
};

int ref (int &x) pre (x > 0) post (r: r == x) { return x; }

__attribute__ ((noipa)) int
use (int x)
{
  S s (x);
  int y = x;
  return h (x) + s.get (1) + ref (y);
}

int
main ()
{
  if (use (3) != 4 + 4 + 3)
    __builtin_abort ();
}
