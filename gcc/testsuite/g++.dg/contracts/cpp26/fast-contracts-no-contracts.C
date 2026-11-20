// -ffast-contracts has no effect without contracts.
// { dg-do compile }
// { dg-options "-ffast-contracts -fno-contracts" }
// { dg-warning "has no effect" "" { target *-*-* } 0 }
