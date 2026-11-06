// A group named by more than one `policy (group)' term gets
// -Wredundant-policy.
// { dg-do compile { target c++26 } }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
// { dg-options "-Wredundant-policy" }

#include <contracts>

[[policy::remove(grp_a)]] void unused_a ();
[[policy::remove(grp_b)]] void unused_b ();

void f (int x) pre <policy(grp_a) | policy(grp_a)> (x > 0); // { dg-warning "policy .grp_a. is named more than once" }
void g (int x) pre <policy(grp_a) | policy(grp_b) | policy(grp_a)> (x > 0); // { dg-warning "policy .grp_a. is named more than once" }
void h (int x) pre <policy(grp_a) | policy(grp_b)> (x > 0);
