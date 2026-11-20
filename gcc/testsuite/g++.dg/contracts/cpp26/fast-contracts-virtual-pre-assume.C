// -ffast-contracts with P3097: a function that overrides B::f assumes at
// entry that its own preconditions or B::f's hold, since any call that
// reaches it checked one of them.  Where it widens B::f's preconditions,
// its own check goes; where it narrows them, the check stays.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }

struct B { virtual int f (int x) pre (x > 0); };
struct Wide : B { int f (int x) override pre (x > -5); };
struct Same : B { int f (int x) override pre (x > 0); };
struct Narrow : B { int f (int x) override pre (x > 10); };

int Wide::f (int x) { return x; }
int Same::f (int x) { return x; }
int Narrow::f (int x) { return x; }

// { dg-final { scan-assembler-not {\n_ZN4Wide1fEi:\n(?:(?!\t\.size)[^\n]*\n)*?\t(?:call|jmp)\t__cxa_contract_violation_pre} } }
// { dg-final { scan-assembler-not {\n_ZN4Same1fEi:\n(?:(?!\t\.size)[^\n]*\n)*?\t(?:call|jmp)\t__cxa_contract_violation_pre} } }
// { dg-final { scan-assembler {\n_ZN6Narrow1fEi:\n(?:(?!\t\.size)[^\n]*\n)*?\t(?:call|jmp)\t__cxa_contract_violation_pre} } }
