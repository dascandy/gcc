// -ffast-contracts sets -fcontracts-client-check=pre: a caller checks the
// precondition in a caller-side wrapper, which calls the postcheck entry
// point.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -ffast-contracts -O0 -fno-asynchronous-unwind-tables" }

int f (int x) pre (x > 0) post (r: r > 0) { return x; }

int use (int x) { return f (x); }

// { dg-final { scan-assembler {\n_Z1fi\.contract_wrapper:\n(?:(?!\.size)[^\n]*\n)*?\t(?:call|jmp)\t_Z1fi\.postcheck\n} } }
// The wrapper checks only the precondition; the postcondition is checked
// in the callee.
// { dg-final { scan-assembler-not {\n_Z1fi\.contract_wrapper:\n(?:(?!\.size)[^\n]*\n)*?\tcall\t__cxa_contract_violation_post} } }
// { dg-final { scan-assembler {_Z1fi\.postcheck:\n(?:[^\n]*\n)*?\tcall\t__cxa_contract_violation_post_enforce} } }
