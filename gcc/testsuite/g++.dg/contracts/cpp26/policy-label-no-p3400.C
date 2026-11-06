// Without -fcontracts-p3400 a label that is nothing but `policy (group)'
// terms is allowed; a P3400 label next to them is not.
// { dg-do compile { target c++26 } }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

[[policy::remove(grp_a)]] void unused_a ();
[[policy::remove(grp_b)]] void unused_b ();

void a (int x) pre <policy(grp_a)> (x > 0);
void b (int x) pre <policy(grp_a) | policy(grp_b)> (x > 0);
void c (int x) { contract_assert <policy(grp_a)> (x > 0); }
void d (int x) pre <lbl | policy(grp_a)> (x > 0); // { dg-error "assertion-control labels require .-fcontracts-p3400." }
