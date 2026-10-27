// { dg-do compile { target c++20 } }
// Subset no_boolean_ordering disallows the built-in <, <=, >, >= and <=> on
// an operand of type bool.

#include <compare>

struct V
{
  bool operator< (bool) const;
};
bool operator> (bool, V);

void
inactive (bool a, bool b)
{
  (void) (a < b);
}

[[subset::activate(no_boolean_ordering)]];

void
active (bool a, bool b, int i, V v, bool &ra)
{
  bool r;
  r = a < b;					// { dg-error "ordering comparison '<' on a 'bool' operand in subset 'no_boolean_ordering'" }
  r = a <= b;					// { dg-error "ordering comparison '<=' on a 'bool' operand" }
  r = a > b;					// { dg-error "ordering comparison '>' on a 'bool' operand" }
  r = a >= b;					// { dg-error "ordering comparison '>=' on a 'bool' operand" }
  r = i < a;					// { dg-error "ordering comparison '<' on a 'bool' operand" }
  r = ra > 0;					// { dg-error "ordering comparison '>' on a 'bool' operand" }
  r = (i < 1) < (i > 0);			// { dg-error "ordering comparison '<' on a 'bool' operand" }
  (void) sizeof (a >= b);			// { dg-error "ordering comparison '>=' on a 'bool' operand" }
  (void) (a <=> b);				// { dg-error "ordering comparison '<=>' on a 'bool' operand" }

  // Allowed: equality and the other operators on bool, ordering of other
  // types, and overloaded operators taking a bool.
  r = a == b;
  r = a != b;
  r = a && b;
  r = a & b;
  r = i < 3;
  r = i >= 3;
  r = v < a;
  r = a > v;
  (void) (i <=> 3);
  (void) r;
}

// In a template, it is checked when instantiated.
template <typename T>
bool
tf (T a, T b)
{
  (void) (a <=> b);				// { dg-error "ordering comparison '<=>' on a 'bool' operand" }
  return a < b;					// { dg-error "ordering comparison '<' on a 'bool' operand" }
}
template bool tf (bool, bool);
template bool tf (int, int);

// The rule does not change which overload is chosen.
template <typename T>
auto pick (T t) -> decltype (t < t, 1) { return 1; }
long pick (...) { return 2; }
static_assert (sizeof (pick (true)) == sizeof (int));

[[subset::suppress(no_boolean_ordering)]];

bool
again (bool a, bool b)
{
  return a < b;
}
