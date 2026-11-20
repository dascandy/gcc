// -ffast-contracts with P3097: a destructor that overrides B::~B assumes at
// entry that its own preconditions or B::~B's hold; widening them, its own
// check goes.  The destruction of its B base still checks B::~B's.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2 -fno-inline" }

struct B { int v = 1; virtual ~B () pre (v > 0); };
struct D : B { ~D () override pre (v > -5); };
D::~D () {}

// D::~D's base-object destructor checks nothing but calls B::~B's
// wrapper, which checks B::~B's precondition.
// { dg-final { scan-assembler-not {\n_ZN1DD2Ev:\n(?:(?!\t\.size)[^\n]*\n)*?\t(?:call|jmp)\t__cxa_contract_violation_pre} } }
// { dg-final { scan-assembler {\n_ZN1DD2Ev:\n(?:(?!\t\.size)[^\n]*\n)*?\t(?:call|jmp)\t_ZN1BD2Ev\.contract_wrapper} } }
