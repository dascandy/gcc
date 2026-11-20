// -fcontract-postcheck-entry: with -ffunction-sections, the postcheck entry
// point gets a section of its own, and for an inline function a COMDAT
// group of its own, so that the linker can keep it and discard the
// function, which callers that check the preconditions never call.  The
// postcheck entry point of a constructor variant that is an alias of
// another goes in the group of that one's.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -O2 -ffunction-sections" }

int f (int x) pre (x > 0) { return x + 1; }
__attribute__((noinline)) inline int il (int x) pre (x > 0) { return x * 2; }
struct S { __attribute__((noinline)) S (int x) pre (x > 0) : v (x) {} int v; };
int use (int x) { return il (x) + f (x) + S (x).v; }

// { dg-final { scan-assembler {\.section\t\.text\._Z1fi,"ax",@progbits} } }
// { dg-final { scan-assembler {\.section\t\.text\._Z1fi\.postcheck,"ax",@progbits} } }
// { dg-final { scan-assembler {\.section\t\.text\._Z2ili,"axG",@progbits,_Z2ili,comdat} } }
// { dg-final { scan-assembler {\.section\t\.text\._Z2ili\.postcheck,"axG",@progbits,_Z2ili\.postcheck,comdat} } }
// { dg-final { scan-assembler {\.weak\t_Z2ili\.postcheck} } }
// { dg-final { scan-assembler {\.section\t\.text\._ZN1SC2Ei,"axG",@progbits,_ZN1SC5Ei,comdat} } }
// { dg-final { scan-assembler {\.section\t\.text\._ZN1SC2Ei\.postcheck,"axG",@progbits,_ZN1SC5Ei\.postcheck,comdat} } }
// { dg-final { scan-assembler {\.set\t_ZN1SC1Ei\.postcheck,_ZN1SC2Ei\.postcheck} } }
