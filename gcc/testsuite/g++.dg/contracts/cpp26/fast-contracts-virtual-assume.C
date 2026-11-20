// -ffast-contracts: a virtual call checks the preconditions of the function
// it names and assumes, rather than checks, its postconditions, also ones
// about the object.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }

void dead ();

struct B
{
  int v;
  virtual int f (int x) pre (x > 0) post (r: r > 0);
  virtual void set (int x) pre (x > 0) post (v > 0);
};

int
use_f (B *p, int x)
{
  int r = p->f (x);
  if (r <= 0)
    dead ();
  return r;
}

int
use_set (B &b, int x)
{
  b.set (x);
  if (b.v <= 0)
    dead ();
  return b.v;
}

// { dg-final { scan-assembler-times {__cxa_contract_violation_pre_enforce} 2 } }
// { dg-final { scan-assembler-not {__cxa_contract_violation_post} } }
// { dg-final { scan-assembler-not {_Z4deadv} } }
