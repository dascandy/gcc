// -fcontract-postcheck-entry: f checks its precondition and ends in a jump
// to f.postcheck, which comes right after it and holds the body and the
// postcondition check.  A function returning a class through a hidden
// pointer forwards that pointer and jumps too.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -fcontract-evaluation-semantic=enforce -O2" }

int f (int x) pre (x > 0) post (r: r > 1) { return x + 1; }

struct S { int v; S (int); S (const S &); ~S (); };
S g (const S s) pre (s.v > 0) post (r: r.v == s.v) { return s; }

// { dg-final { scan-assembler {\n_Z1fi:\n} } }
// { dg-final { scan-assembler {\tjmp\t_Z1fi\.postcheck\n(?:\t\.cfi[^\n]*\n)*\.LFE[0-9]+:\n\t\.size\t_Z1fi, \.-_Z1fi\n(?:\t\.p2align[^\n]*\n)*\t\.globl\t_Z1fi\.postcheck\n\t\.type\t_Z1fi\.postcheck, @function\n_Z1fi\.postcheck:\n} } }
// { dg-final { scan-assembler {\tjmp\t_Z1g1S\.postcheck\n(?:\t\.cfi[^\n]*\n)*\.LFE[0-9]+:\n\t\.size\t_Z1g1S, \.-_Z1g1S\n} } }
// The precondition is checked only at the first entry point, the
// postcondition only at the second.
// { dg-final { scan-assembler-times {__cxa_contract_violation_pre_enforce} 2 } }
// { dg-final { scan-assembler {_Z1fi\.postcheck:\n(?:[^\n]*\n)*?\tcall\t__cxa_contract_violation_post_enforce} } }
