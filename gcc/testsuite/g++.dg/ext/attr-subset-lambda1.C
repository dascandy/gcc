// { dg-do compile { target c++20 } }
// A subset activated in the body of a generic lambda in a template applies
// when the lambda is instantiated.

#include <exception>

template <typename T>
void
f ()
{
  auto l = [] (auto u) {
    [[subset::activate(throw_non_std_exception)]];
    throw u;						// { dg-error "throwing 'int'" }
  };
  l (1);
  throw T ();
}

template void f<long> ();
