// A `policy (group)' term naming a group that is still not known at the end
// of the translation unit gets -Wunknown-policy, at the group name; a group
// that becomes known later does not.
// { dg-do compile { target c++26 } }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

[[policy::remove(grp_a)]] void unused_a ();

void f (int x) pre <policy(grp_aa)> (x > 0); // { dg-warning "28: unknown policy .grp_aa.; did you mean .grp_a.?" }
void g (int x) pre <policy(grp_later)> (x > 0);
void h (int x) pre <policy(nothing_like_it)> (x > 0); // { dg-warning "unknown policy .nothing_like_it." }

[[policy::remove(grp_later)]] void unused_later ();
