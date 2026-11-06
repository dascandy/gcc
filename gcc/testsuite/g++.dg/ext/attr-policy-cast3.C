// { dg-do compile { target c++20 } }
// A C-style cast in a template is checked when the template is instantiated,
// since only then is it known what it does, against the policies active
// where the template is defined.  Whether a substitution fails does not
// change.  A C-style cast in a system header is never checked.

template<typename T>
long
off (T *p)
{
  return (long) p;
}

[[policy::activate(reinterpret_cast), policy::activate(const_cast)]];

#include "attr-policy-cast3.h"

template<typename T>
long
on (T *p)
{
  return (long) p;		// { dg-error "effect of 'reinterpret_cast'" }
}

// Only a reinterpret_cast for some T.
template<typename T>
T
dep (int *p)
{
  return (T) p;			// { dg-error "effect of 'reinterpret_cast'" }
}

template<typename T>
T *
away (const T *p)
{
  return (T *) p;		// { dg-error "casts away qualifiers" }
}

// Never instantiated: not checked.
template<typename T>
long
unused (T *p)
{
  return (long) p;
}

// The cast in the constraint is only formed during substitution, where it
// does not make the substitution fail or give an error.
template<typename T>
  requires requires (T *p) { (long) p; }
constexpr int
sf ()
{
  return 1;
}

[[policy::suppress(reinterpret_cast), policy::suppress(const_cast)]];

void
use (int *p, const int *cp)
{
  off (p);
  on (p);
  dep<void *> (p);
  dep<long> (p);
  away (cp);
  sys_r (p);
  sys_c (cp);
  sys_tr (p);
  sys_tc (cp);
  static_assert (sf<int> () == 1);
}
