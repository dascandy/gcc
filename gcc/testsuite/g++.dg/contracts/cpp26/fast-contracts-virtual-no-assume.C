// Without -ffast-contracts a virtual call checks the postconditions of the
// function it names (P3097).
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -O2" }

struct B { virtual int f (int x) pre (x > 0) post (r: r > 0); };

int use_f (B *p, int x) { return p->f (x); }

// { dg-final { scan-assembler {__cxa_contract_violation_post_enforce} } }
