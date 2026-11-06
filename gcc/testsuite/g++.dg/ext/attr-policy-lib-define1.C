// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
// A user grouping can include the predefined unicode grouping, and a
// predefined grouping cannot be redefined or removed under.
#include <iostream>

[[policy::define(mine, unicode, endl)]];
[[policy::define(unicode, cout)]];		// { dg-error "policy 'unicode' is already known" }
[[policy::remove(unicode)]] void h ();	// { dg-error "defined by 'policy::define'" }

void
f ()
{
  [[policy::activate(mine)]];
  std::cout << std::endl;	// { dg-error "'std::cout' was policy-removed" }
				// { dg-error "endl.*was policy-removed" "" { target *-*-* } .-1 }
  std::clog << "x";
}
