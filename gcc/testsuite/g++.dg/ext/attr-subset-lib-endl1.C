// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
#include <ostream>
#include <iostream>

void
f ()
{
  [[subset::activate(endl)]];
  std::cout << std::endl;	// { dg-error "std::endl\\(\[^'\]*' was subset-removed" }
  std::cout << '\n';
}

void
g ()
{
  std::cout << std::endl;
}

// Subset inefficient removes std::endl.
void
h ()
{
  [[subset::activate(inefficient)]];
  std::cout << std::endl;	// { dg-error "std::endl\\(\[^'\]*' was subset-removed" }
  std::cout << '\n' << std::flush;
}
