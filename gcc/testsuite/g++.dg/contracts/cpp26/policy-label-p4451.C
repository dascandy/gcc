// Under -fcontracts-p4451 without -fcontracts-p3400, a `policy (group)'
// term combines with `terminating', in either order, and a label that is
// nothing but terms is allowed.  Whatever else is left once the terms are
// taken out has to be `terminating'.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p4451" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

[[policy::remove(grp_a)]] void unused_a ();
[[policy::remove(grp_b)]] void unused_b ();
[[policy::activate(grp_a)]];

int a (int x) pre <terminating | policy(grp_a)> (x > 0) { return x; }
int b (int x) pre <policy(grp_a) | terminating> (x > 0) { return x; }
int c (int x) pre <policy(grp_a) | policy(grp_b)> (x > 0) { return x; }
void d (int x) { contract_assert <policy(grp_b) | terminating> (x > 0); }

int e (int x) pre <other | policy(grp_a)> (x > 0) { return x; } // { dg-error "only .<terminating>. is allowed" }
