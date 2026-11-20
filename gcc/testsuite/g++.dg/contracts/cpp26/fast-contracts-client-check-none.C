// -ffast-contracts makes callers check the preconditions, and only those.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -ffast-contracts -fcontracts-client-check=none" }
// { dg-error "'-ffast-contracts' requires '-fcontracts-client-check=pre', not '-fcontracts-client-check=none'" "" { target *-*-* } 0 }
