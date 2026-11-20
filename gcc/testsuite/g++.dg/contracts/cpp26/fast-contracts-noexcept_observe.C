// -ffast-contracts only works with an enforcing evaluation semantic.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -ffast-contracts -fcontract-evaluation-semantic=noexcept_observe -fcontracts-p4298" }
// { dg-error "'-ffast-contracts' only works with an enforcing contract evaluation semantic \\('enforce', 'quick_enforce' or 'noexcept_enforce'\\), not '-fcontract-evaluation-semantic=noexcept_observe'" "" { target *-*-* } 0 }
