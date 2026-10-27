// { dg-do compile { target c++20 } }
// Member templates of class templates, and generic lambdas in templates,
// use the subsets that were active at their definition, whatever is active
// where they are instantiated.

#include <exception>

#if !__has_cpp_attribute(subset::remove)
#error "subset::remove not supported"
#endif
#if !__has_cpp_attribute(subset::activate)
#error "subset::activate not supported"
#endif
#if !__has_cpp_attribute(subset::suppress)
#error "subset::suppress not supported"
#endif
#if !__has_cpp_attribute(subset::define)
#error "subset::define not supported"
#endif
#if __has_cpp_attribute(subset::unknown)
#error "subset::unknown supported"
#endif

[[subset::activate(throw_non_std_exception)]];
template <typename T>
struct A
{
  template <typename U> void g () { throw U (); }	// { dg-error "throwing 'int'" }
  template <typename U> struct B
  {
    void h () { throw U (); }				// { dg-error "throwing 'long int'" }
  };
};
template <typename T>
void
b ()
{
  auto l = [] (auto u) { throw u; };			// { dg-error "throwing 'int'" }
  l (1);
}
template <typename T>
void
c ()
{
  auto l = [] <typename U> (U u) { throw u; };		// { dg-error "throwing 'char'" }
  l ('c');
}
[[subset::suppress(throw_non_std_exception)]];

template <typename T>
struct A2
{
  template <typename U> void g () { throw U (); }
};
template <typename T>
void
b2 ()
{
  auto l = [] (auto u) { throw u; };
  l (1);
}

// A generic lambda sees the subsets activated in the template before it,
// and its own changes do not leak out of it.
template <typename T>
void
d ()
{
  [[subset::activate(throw_non_std_exception)]];
  auto l = [] (auto u) { throw u; };			// { dg-error "throwing 'short int'" }
  l (short (2));
}
template <typename T>
void
e ()
{
  auto l = [] (auto u) {
    [[subset::activate(throw_non_std_exception)]];
    throw u;						// { dg-error "throwing 'unsigned int'" }
  };
  l (3u);
  throw T ();
}

void
use ()
{
  A<int> ().g<int> ();
  A<int>::B<long> ().h ();
  b<int> ();
  c<int> ();
}

void
use_active ()
{
  [[subset::activate(throw_non_std_exception)]];
  A2<int> ().g<int> ();
  b2<int> ();
  d<int> ();
  e<int> ();
}
