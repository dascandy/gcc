// { dg-do compile { target c++17 } }
// Throwing something other than an object of a class derived from
// std::exception is an error when subset throw_non_std_exception is active.

#include <stdexcept>

struct D : std::runtime_error { D () : std::runtime_error ("d") {} };
struct P : private std::exception {};
struct N {};
enum E { e };

void
inactive ()
{
  throw 1;
  throw N ();
}

void
active (int i)
{
  [[subset::activate(throw_non_std_exception)]];
  static D d;
  const D cd;
  std::exception *pe = &d;

  // Objects of classes derived from std::exception.
  throw std::exception ();
  throw std::logic_error ("x");
  throw D ();
  throw d;
  throw cd;
  throw *pe;
  throw P ();
  try { throw D (); } catch (...) { throw; }

  // Anything else.
  throw 1;			// { dg-error "throwing 'int', which is not derived from 'std::exception'" }
  throw N ();			// { dg-error "throwing 'N'" }
  throw e;			// { dg-error "throwing 'E'" }
  throw "x";			// { dg-error "throwing 'const char\\*'" }
  throw &d;			// { dg-error "throwing 'D\\*'" }
  throw pe;			// { dg-error "throwing 'std::exception\\*'" }
  throw nullptr;		// { dg-error "throwing 'std::nullptr_t'" }
  throw i ? D () : D ();
  {
    [[subset::suppress(throw_non_std_exception)]];
    throw 2;
  }
}

// Templates use the subsets active at their definition.
template <typename T> void t1 () { throw T (); }
[[subset::activate(throw_non_std_exception)]];
template <typename T> void t2 () { throw T (); }	// { dg-error "throwing 'int'" }
[[subset::suppress(throw_non_std_exception)]];

void
use ()
{
  [[subset::activate(throw_non_std_exception)]];
  t1<int> ();
  t2<int> ();
  t2<D> ();
}

// Overload resolution is the same whether the subset is active or not.
[[subset::activate(throw_non_std_exception)]];
template <typename T> auto sf (T t) -> decltype (throw t, 1) { return 1; }
long sf (...) { return 2; }
static_assert (sizeof (sf (1)) == sizeof (int));
template <typename T, typename = decltype (throw T ())> char sg (T);
long sg (...);
static_assert (sizeof (sg (N ())) == sizeof (char));
[[subset::suppress(throw_non_std_exception)]];
