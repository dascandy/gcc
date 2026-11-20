// -fcontract-postcheck-entry: only the postcheck entry point, which holds
// the body, is instrumented.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -O2 -finstrument-functions -fno-asynchronous-unwind-tables" }

int f (int x) pre (x > 0) { return x + 1; }

// { dg-final { scan-assembler-not {\n_Z1fi:\n(?:(?!_Z1fi\.postcheck:).)*__cyg_profile_func_enter} } }
// { dg-final { scan-assembler {\n_Z1fi\.postcheck:\n(?:[^\n]*\n)*?\tcall\t__cyg_profile_func_enter} } }
