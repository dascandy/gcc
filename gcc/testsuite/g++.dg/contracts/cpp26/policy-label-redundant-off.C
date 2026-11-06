// -Wredundant-policy is not on by default (the default options of the
// testsuite, -pedantic-errors, enable it).
// { dg-do compile { target c++26 } }
// { dg-options "" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

[[policy::remove(grp_a)]] void unused_a ();

void f (int x) pre <policy(grp_a) | policy(grp_a)> (x > 0); // { dg-bogus "named more than once" }
