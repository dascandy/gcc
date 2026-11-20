// -ffast-contracts with P3097: a postcondition whose predicate cannot be
// rewritten in the functions that override it -- here one that creates a
// temporary -- is not checked by them, so a virtual call checks it rather
// than assume it.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }

struct T { int v; T (int v) : v (v) {} ~T (); bool ok () const { return v > 0; } };
struct B { virtual int f (int x) pre (x > 0) post (r: T (r).ok ()); };

int use (B *p, int x) { return p->f (x); }

// { dg-final { scan-assembler {__cxa_contract_violation_post_enforce} } }
