// { dg-do compile { target c++20 } }
// A default member initializer of a class template uses the subsets that
// were active at its definition, like a function body.  A default argument
// uses the subsets active where it is used.

#include <exception>

[[subset::activate(throw_non_std_exception)]];
template <typename T>
struct D
{
  int i = (throw T (), 0);			// { dg-error "throwing '(long )?int'" }
  void f (int = (throw T (), 0));		// { dg-error "throwing 'long int'" }
};
[[subset::suppress(throw_non_std_exception)]];

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
  [[subset::activate(throw_non_std_exception)]];
  D<long> d;
  d.f ();
  D2<long> d2;
  d2.f ();
}
