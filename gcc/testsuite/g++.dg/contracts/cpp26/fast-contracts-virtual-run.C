// -ffast-contracts: virtual calls that assume the postconditions of the
// function they name behave as without the assumptions, at run time and
// in constant evaluation, where the call still checks them.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }

struct B
{
  int v = 0;
  virtual int f (int x) pre (x > 0) post (r: r > 0) { return x; }
  virtual void set (int x) pre (x > 0) post (v > 0) { v = x; }
  constexpr virtual int g (const int x) const pre (x > 0) post (r: r > x) { return x + 1; }
};

struct D : B
{
  int f (int x) override { return x + 1; }
  void set (int x) override { v = x + 1; }
  constexpr int g (const int x) const override { return x + 2; }
};

constexpr int
cg (const B &b, int x)
{
  return b.g (x);
}

static_assert (cg (B {}, 1) == 2);
static_assert (cg (D {}, 1) == 3);

__attribute__ ((noipa)) int
use (B *p, int x)
{
  p->set (x);
  return p->f (x) + p->v + p->g (x);
}

int
main ()
{
  B b;
  D d;
  if (use (&b, 1) != 1 + 1 + 2 || use (&d, 1) != 2 + 2 + 3)
    __builtin_abort ();
}
