// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
#include <ostream>
#include <iostream>

void
f ()
{
  [[policy::activate(endl)]];
  std::cout << std::endl;	// { dg-error "std::endl\\(\[^'\]*' was policy-removed" }
  std::cout << '\n';
}

void
g ()
{
  std::cout << std::endl;
}

// Policy inefficient removes std::endl.
void
h ()
{
  [[policy::activate(inefficient)]];
  std::cout << std::endl;	// { dg-error "std::endl\\(\[^'\]*' was policy-removed" }
  std::cout << '\n' << std::flush;
}
