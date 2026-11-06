// { dg-do compile { target c++20 } }
// A default member initializer of a class template uses the policies that
// were active at its definition, like a function body.  A default argument
// uses the policies active where it is used.

#include <exception>

[[policy::activate(throw_non_std_exception)]];
template <typename T>
struct D
{
  int i = (throw T (), 0);			// { dg-error "throwing '(long )?int'" }
  void f (int = (throw T (), 0));		// { dg-error "throwing 'long int'" }
};
[[policy::suppress(throw_non_std_exception)]];

template <typename T>
struct D2
{
  int i = (throw T (), 0);
  void f (int = (throw T (), 0));		// { dg-error "throwing 'long int'" }
};

void
inactive ()
{
  D<int> d;
  d.f ();
  D2<int> d2;
  d2.f ();
}

void
active ()
{
  [[policy::activate(throw_non_std_exception)]];
  D<long> d;
  d.f ();
  D2<long> d2;
  d2.f ();
}
