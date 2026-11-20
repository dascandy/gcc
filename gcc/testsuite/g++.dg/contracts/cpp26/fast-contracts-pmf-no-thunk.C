// Without -ffast-contracts a pointer to a virtual member function with a
// precondition has the usual layout of a pointer to a virtual function.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -O2" }

struct B { virtual int f (int x) pre (x > 0); };

int (B::*make_f ()) (int) { return &B::f; }

// { dg-final { scan-assembler {\n_Z6make_fv:\n(?:(?!\t\.size)[^\n]*\n)*?\tmovl\t\$1, } } }
// { dg-final { scan-assembler-not {contract_virtual} } }
