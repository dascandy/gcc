// -ffast-contracts: a virtual call assumes only the postconditions whose
// semantic enforces them; one that is observed, or has captures, is still
// checked by the call.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p3098 -ffast-contracts -O2" }
// { dg-additional-options {-fcontract-configuration=\[{"match":{"caller":false,"kind":"post","namespace":"ob"},"output":{"semantic":"observe"}}\]} }

namespace ob { struct B { virtual int f (int x) pre (x > 0) post (r: r > 0); }; }
namespace cap { struct B { virtual int f (int x) pre (x > 0) post [x] (r: r > x); }; }

int use_ob (ob::B *p, int x) { return p->f (x); }
int use_cap (cap::B *p, int x) { return p->f (x); }

// { dg-final { scan-assembler {__cxa_contract_violation_post_observe} } }
// { dg-final { scan-assembler {__cxa_contract_violation_post_enforce} } }
