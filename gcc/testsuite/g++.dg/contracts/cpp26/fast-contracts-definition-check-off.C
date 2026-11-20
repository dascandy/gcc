// -ffast-contracts makes callees check the postconditions.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -ffast-contracts -fcontracts-definition-check=off" }
// { dg-error "'-ffast-contracts' requires '-fcontracts-definition-check=on'" "" { target *-*-* } 0 }
