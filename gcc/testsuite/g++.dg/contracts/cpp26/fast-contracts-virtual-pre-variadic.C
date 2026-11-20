// -ffast-contracts with P3097: a C-variadic function that overrides others
// makes no assumption at entry -- a pointer to member function made from
// it, or from one it overrides, does not check their preconditions -- so
// it keeps its own precondition check even where it widens them.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }

struct B { virtual int f (int x, ...) pre (x > 0); };
struct D : B { int f (int x, ...) override pre (x > -5); };
int D::f (int x, ...) { return x; }

// { dg-final { scan-assembler {\n_ZN1D1fEiz:\n(?:(?!\t\.size)[^\n]*\n)*?\t(?:call|jmp)\t__cxa_contract_violation_pre} } }
