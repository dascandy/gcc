// -fcontract-postcheck-entry: a function whose preconditions are not
// checked in this translation unit still has a postcheck entry point, for
// callers elsewhere; it is an alias of the function.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -fcontracts-definition-check=off" }

int f (int x) pre (x > 0) post (r: r > 0) { return x; }

// { dg-final { scan-assembler {\.set\t_Z1fi\.postcheck,_Z1fi} } }
