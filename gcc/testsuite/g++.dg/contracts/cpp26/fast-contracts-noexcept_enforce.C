// -ffast-contracts accepts the enforcing semantic noexcept_enforce.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -ffast-contracts -fcontract-evaluation-semantic=noexcept_enforce -fcontracts-p4298" }

int f (int x) pre (x > 0) { return x; }

// { dg-final { scan-assembler {_Z1fi\.postcheck:} } }
