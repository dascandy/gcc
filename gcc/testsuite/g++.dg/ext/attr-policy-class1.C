// { dg-do compile { target c++20 } }
// A policy activated or suppressed in a class applies from that point to
// the end of the class, including to the members whose bodies, default
// arguments and initializers are parsed after the class is complete.

#include <exception>

struct S
{
  void before () { ; }
  int i1 = ([] { ; } (), 0);
  [[policy::activate(no_empty_statement)]];
  void after () { ; }				// { dg-error "empty statement" }
  int i2 = ([] { ; } (), 0);			// { dg-error "empty statement" }
  void defarg (int = ([] { ; } (), 0));		// { dg-error "empty statement" }
  void noex () noexcept (([] { ; } (), true));	// { dg-error "empty statement" }
  struct N
  {
    void f () { ; }				// { dg-error "empty statement" }
  };
  [[policy::suppress(no_empty_statement)]];
  void later () { ; }
  struct N2
  {
    [[policy::activate(no_empty_statement)]];
    void f () { ; }				// { dg-error "empty statement" }
  };
  void after_nested () { ; }
};

// The changes do not leak out of the class.
[[policy::activate(no_empty_statement)]];
struct S2
{
  [[policy::suppress(no_empty_statement)]];
  void f () { ; }
};
void g () { ; }					// { dg-error "empty statement" }
[[policy::suppress(no_empty_statement)]];

// A class template replays the changes when it is instantiated, for its
// member functions and for the members it instantiates with it.
template <typename T>
struct CT
{
  void before () { throw T (); }
  [[policy::activate(throw_non_std_exception)]];
  void after () { throw T (); }			// { dg-error "throwing 'int'" }
  [[policy::suppress(throw_non_std_exception)]];
  void later () { throw T (); }
};

template <typename T>
struct CT2
{
  using type1 = decltype (throw T (), 0);
  [[policy::activate(throw_non_std_exception)]];
  using type2 = decltype (throw T (), 0);	// { dg-error "throwing 'int'" }
  [[policy::suppress(throw_non_std_exception)]];
  using type3 = decltype (throw T (), 0);
};

void
use ()
{
  CT<int> c;
  c.before ();
  c.after ();
  c.later ();
  CT2<int> c2;
}

// Errors.
struct E
{
  [[policy::activate(unknown)]];		// { dg-error "unknown policy 'unknown'" }
  [[policy::define(local, no_empty_statement)]];	// { dg-error "must appear at namespace scope" }
  [[policy::activate(no_empty_statement)]] int x;	// { dg-warning "'policy::activate' attribute must appear as a separate statement or declaration" }
};
