// -fcontract-postcheck-entry has no effect without contracts.
// { dg-do compile }
// { dg-options "-fcontract-postcheck-entry -fno-contracts" }
// { dg-warning "has no effect" "" { target *-*-* } 0 }
