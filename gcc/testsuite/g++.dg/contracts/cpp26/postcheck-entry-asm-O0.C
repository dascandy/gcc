// -fcontract-postcheck-entry: the call from the first entry point to the
// second is a sibling call even at -O0, as a thunk's is.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -fcontract-evaluation-semantic=enforce -O0" }

int f (int x) pre (x > 0) { return x + 1; }

// { dg-final { scan-assembler {\tjmp\t_Z1fi\.postcheck\n(?:\t\.cfi[^\n]*\n)*\.LFE[0-9]+:\n\t\.size\t_Z1fi, \.-_Z1fi\n\t\.globl\t_Z1fi\.postcheck\n} } }
