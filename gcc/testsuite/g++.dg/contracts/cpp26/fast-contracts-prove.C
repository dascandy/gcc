// -ffast-contracts only works with an enforcing evaluation semantic.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -ffast-contracts -fcontract-evaluation-semantic=prove" }
// { dg-error "'-ffast-contracts' only works with an enforcing contract evaluation semantic \\('enforce', 'quick_enforce' or 'noexcept_enforce'\\), not '-fcontract-evaluation-semantic=prove'" "" { target *-*-* } 0 }
