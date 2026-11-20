// -ffast-contracts: the caller-side wrapper's assumption of a postcondition
// whose predicate calls a function evaluates the predicate, calls
// included, whatever their side effects.  The optimizers can use the
// result of a call to a pure or an inline function; a call to a function
// they cannot see is made again.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -ffast-contracts -O2" }

void dead ();
[[gnu::pure]] bool pure_ok (int);
bool ext_ok (int);

struct S
{
  int v;
  int size () const { return v; }
  void set (int x) pre (x > 0) post (size () > 0);
};

int f (int x) pre (x > 0) post (r: pure_ok (r));
int g (int x) pre (x > 0) post (r: ext_ok (r));

int
use_pure (int x)
{
  int r = f (x);
  if (!pure_ok (r))
    dead ();
  return r;
}

int
use_inline (S &s, int x)
{
  s.set (x);
  if (s.size () <= 0)
    dead ();
  return s.v;
}

int
use_ext (int x)
{
  return g (x);
}

// { dg-final { scan-assembler-not {_Z4deadv} } }
// The assumption in use_ext calls ext_ok once.
// { dg-final { scan-assembler-times {\t(?:call|jmp)\t_Z6ext_oki\n} 1 } }
