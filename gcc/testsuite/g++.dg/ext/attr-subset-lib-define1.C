// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
// A user grouping can include the predefined unicode grouping, and a
// predefined grouping cannot be redefined or removed under.
#include <iostream>

[[subset::define(mine, unicode, endl)]];
[[subset::define(unicode, cout)]];		// { dg-error "subset 'unicode' is already known" }
[[subset::remove(unicode)]] void h ();	// { dg-error "defined by 'subset::define'" }

void
f ()
{
  [[subset::activate(mine)]];
  std::cout << std::endl;	// { dg-error "'std::cout' was subset-removed" }
				// { dg-error "std::endl\\(\[^'\]*' was subset-removed" "" { target *-*-* } .-1 }
  std::clog << "x";		// { dg-error "'std::clog' was subset-removed" }
}
