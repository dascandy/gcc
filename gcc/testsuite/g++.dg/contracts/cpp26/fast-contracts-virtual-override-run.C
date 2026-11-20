// -ffast-contracts with P3097: functions that override others assume one
// of their preconditions at entry and check their postconditions where a
// call may have come through them: through either base class of a class
// with two, for a covariant return, and with no violation where none is
// due -- a call through B2 does not have B1's postconditions checked.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }

struct B1 { virtual int f (int x) pre (x > 0) post (r: r > 0); };
struct B2 { virtual int f (int x) pre (x < 0) post (r: r < 0); };
struct D : B1, B2 { int f (int x) override; };
int B1::f (int x) { return x; }
int B2::f (int x) { return x; }
int D::f (int x) { return x; }

struct A { int a = 7; };
struct C : A { int c = 1; };
struct P { virtual A *g (int x) pre (x > 0) post (r: r != nullptr && r->a == 7); };
struct Q : P { C *g (int x) override; };
C the_c;
A *P::g (int) { return &the_c; }
C *Q::g (int) { return &the_c; }

__attribute__ ((noipa)) int via1 (B1 *p, int x) { return p->f (x); }
__attribute__ ((noipa)) int via2 (B2 *p, int x) { return p->f (x); }
__attribute__ ((noipa)) A *viap (P *p, int x) { return p->g (x); }

int
main ()
{
  D d;
  Q q;
  if (via1 (&d, 3) != 3 || via2 (&d, -3) != -3 || d.f (4) != 4)
    __builtin_abort ();
  if (viap (&q, 1) != &the_c || q.g (2) != &the_c)
    __builtin_abort ();
}
