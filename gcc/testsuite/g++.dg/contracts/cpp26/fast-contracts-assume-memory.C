// -ffast-contracts: the caller-side wrapper's assumption of a postcondition
// is made by evaluating the predicate, so the optimizers can use what it
// says about memory: the state of an object after a member function or a
// constructor, also through a pointer.  As [[assume]], such a predicate,
// which could trap, would be outlined and of no use.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -ffast-contracts -O2" }

void dead ();

struct S
{
  int v;
  S (const int x) pre (x > 0) post (v == x);
  void set (int x) pre (x > 0) post (v > 0);
};

int
use_member (S &s, int x)
{
  s.set (x);
  if (s.v <= 0)
    dead ();
  return s.v;
}

int
use_pointer (S *p, int x)
{
  p->set (x);
  if (p->v <= 0)
    dead ();
  return p->v;
}

int
use_ctor (int x)
{
  S s (x);
  if (s.v != x)
    dead ();
  return s.v;
}

int
use_prvalue (int x)
{
  S s = S (x);
  if (s.v != x)
    dead ();
  return s.v;
}

// { dg-final { scan-assembler-not {_Z4deadv} } }
