// Two function contract assertions are the same only if they have the same
// `policy (group)' terms, in any order, each active at both or at neither.
// { dg-do compile { target c++26 } }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

[[policy::remove(grp_a)]] void unused_a ();
[[policy::remove(grp_b)]] void unused_b ();

void f1 (int x) pre <policy(grp_a)> (x > 0);
void f1 (int x) pre (x > 0) { } // { dg-error "mismatched assertion-control-specifier" }

void f2 (int x) pre (x > 0);
void f2 (int x) pre <policy(grp_a)> (x > 0) { } // { dg-error "mismatched assertion-control-specifier" }

void f3 (int x) pre <policy(grp_a)> (x > 0);
void f3 (int x) pre <policy(grp_b)> (x > 0) { } // { dg-error "mismatched assertion-control-specifier" }

void f4 (int x) pre <policy(grp_a) | policy(grp_b)> (x > 0);
void f4 (int x) pre <policy(grp_b) | policy(grp_a)> (x > 0) { }

void f5 (int x) pre <policy(grp_a) | policy(grp_b)> (x > 0);
void f5 (int x) pre <policy(grp_a)> (x > 0) { } // { dg-error "mismatched assertion-control-specifier" }

void f6 (int x) pre <policy(grp_a)> (x > 0); // { dg-message "but is not active at the previous contract" }
[[policy::activate(grp_a)]];
void f6 (int x) pre <policy(grp_a)> (x > 0) { } // { dg-error "mismatched assertion-control-specifier" }
// { dg-message "policy .grp_a. is active here" "" { target *-*-* } .-1 }

void f7 (int x) pre <policy(grp_a)> (x > 0);
void f7 (int x) pre <policy(grp_a)> (x > 0) { }
