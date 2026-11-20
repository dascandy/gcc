// -ffast-contracts enables -fcontract-postcheck-entry: f gets its second
// entry point, and f's own symbol stays for function pointers and callers
// that know nothing of contracts.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -ffast-contracts" }

int f (int x) pre (x > 0) { return x; }

// { dg-final { scan-assembler {_Z1fi\.postcheck:} } }
// { dg-final { scan-assembler {_Z1fi:} } }
