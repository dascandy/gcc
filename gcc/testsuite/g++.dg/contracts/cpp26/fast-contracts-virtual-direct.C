// -ffast-contracts with P3097: a call that names a virtual function without
// dispatching checks its preconditions with their own semantics, as a
// virtual call does, whatever the caller-side configuration says, since
// the functions that override it assume them.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }
// { dg-additional-options {-fcontract-configuration=\[{"match":{"caller":true},"output":{"semantic":"ignore"}}\]} }

struct B { virtual int f (int x) pre (x > 0); };
struct F final : B { int f (int x) override pre (x > 0); };

int use_qualified (B *p, int x) { return p->B::f (x); }
int use_final (F &f, int x) { return f.f (x); }

// { dg-final { scan-assembler-times {__cxa_contract_violation_pre_enforce} 2 } }
