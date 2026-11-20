// -ffast-contracts cannot do without the postcheck entry points.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -ffast-contracts -fno-contract-postcheck-entry" }
// { dg-error "'-ffast-contracts' requires '-fcontract-postcheck-entry'" "" { target *-*-* } 0 }
