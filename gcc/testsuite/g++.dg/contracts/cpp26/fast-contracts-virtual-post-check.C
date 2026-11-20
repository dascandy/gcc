// -ffast-contracts with P3097: a function that overrides B::f checks B::f's
// postconditions, which a virtual call through B::f assumes, on a return
// from a call that may have come through B::f -- also when it has no
// contracts of its own.  Where its own postconditions imply B::f's, only
// its own are checked.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }

int g (int);

struct B { virtual int f (int x) pre (x > 0) post (r: r > 0); };
struct Plain : B { int f (int x) override; };
struct Narrow : B { int f (int x) override post (r: r > 1); };

int Plain::f (int x) { return g (x); }
int Narrow::f (int x) { return g (x); }

// { dg-final { scan-assembler {\n_ZN5Plain1fEi:\n(?:(?!\t\.size)[^\n]*\n)*?\t(?:call|jmp)\t__cxa_contract_violation_post_enforce} } }
// Narrow::f checks r > 1 alone.
// { dg-final { scan-assembler-times {__cxa_contract_violation_post_enforce} 2 } }
