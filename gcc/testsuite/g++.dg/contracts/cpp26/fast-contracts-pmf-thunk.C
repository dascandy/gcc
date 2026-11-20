// -ffast-contracts with P3097: a pointer to member function made from a
// virtual function with a precondition points to a thunk that checks it
// and dispatches, with the layout of a pointer to a non-virtual function.
// The thunk has vague linkage, so that such pointers compare equal across
// translation units.  A virtual function without a precondition, and a
// C-variadic one, keep the layout of a pointer to a virtual function.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }

struct B
{
  virtual int f (int x) pre (x > 0);
  virtual int g (int x);
  virtual int v (int x, ...) pre (x > 0);
};

int (B::*make_f ()) (int) { return &B::f; }
int (B::*make_g ()) (int) { return &B::g; }
int (B::*make_v ()) (int, ...) { return &B::v; }

// A call through a constant pointer checks the precondition.
int via_const (B *b, int x) { int (B::*p) (int) = &B::f; return (b->*p) (x); }

// { dg-final { scan-assembler {\n_Z6make_fv:\n(?:(?!\t\.size)[^\n]*\n)*?\tmov[lq]\t\$_ZN1B1fEi\.contract_virtual, } } }
// { dg-final { scan-assembler {\n_Z6make_gv:\n(?:(?!\t\.size)[^\n]*\n)*?\tmovl\t\$9, } } }
// { dg-final { scan-assembler {\n_Z6make_vv:\n(?:(?!\t\.size)[^\n]*\n)*?\tmovl\t\$17, } } }
// { dg-final { scan-assembler {\t\.weak\t_ZN1B1fEi\.contract_virtual\n} } }
// { dg-final { scan-assembler {\t\.section\t\.text\._ZN1B1fEi\.contract_virtual,"axG",@progbits,_ZN1B1fEi\.contract_virtual,comdat\n} } }
// { dg-final { scan-assembler-not {_ZN1B1vEz\.contract_virtual} } }
// { dg-final { scan-assembler {\n_Z9via_constP1Bi:\n(?:(?!\t\.size)[^\n]*\n)*?\t(?:call|jmp)\t__cxa_contract_violation_pre_enforce} } }
