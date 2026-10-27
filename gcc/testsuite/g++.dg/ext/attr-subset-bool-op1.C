// { dg-do compile { target c++17 } }
// Subset binary_op_on_bool disallows the bitwise operators &, | and ~, and
// &= and |=, on an operand of type bool.

struct V
{
  V operator& (bool) const;
  V &operator|= (bool);
};
V operator| (bool, V);

struct ToBool { operator bool () const { return true; } };

void
inactive (bool a, bool b)
{
  a = a & b;
  a |= b;
  (void) ~a;
}

[[subset::activate(binary_op_on_bool)]];

void
active (bool a, bool b, int i, unsigned u, V v, ToBool tb, bool &ra)
{
  bool r;
  r = a & b;					// { dg-error "bitwise '&' on a 'bool' operand" }
  r = a | b;					// { dg-error "bitwise '\\|' on a 'bool' operand" }
  i = a & i;					// { dg-error "bitwise '&' on a 'bool' operand" }
  i = i | b;					// { dg-error "bitwise '\\|' on a 'bool' operand" }
  r = (i < 1) & (i > 0);			// { dg-error "bitwise '&' on a 'bool' operand" }
  r = ra | b;					// { dg-error "bitwise '\\|' on a 'bool' operand" }
  i = ~a;					// { dg-error "bitwise '~' on a 'bool' operand" }
  a &= b;					// { dg-error "bitwise '&=' on a 'bool' operand" }
  a |= b;					// { dg-error "bitwise '\\|=' on a 'bool' operand" }
  i &= a;					// { dg-error "bitwise '&=' on a 'bool' operand" }
  ra |= true;					// { dg-error "bitwise '\\|=' on a 'bool' operand" }
  (void) sizeof (a & b);			// { dg-error "bitwise '&' on a 'bool' operand" }

  // Allowed: the other operators on bool, the bitwise operators on other
  // types, and overloaded operators taking a bool.
  r = a && b;
  r = a || b;
  r = !a;
  r = a ^ b;
  r = a == b;
  r = a;
  i = i & 1;
  u = u | 2u;
  i = ~i;
  i &= 3;
  u |= 4u;
  v = v & a;
  v = a | v;
  v |= a;
  r = tb & tb;
  (void) r; (void) i; (void) u;
}

// In a template, it is checked when instantiated.
template <typename T>
T
tf (T a, T b)
{
  a |= b;					// { dg-error "bitwise '\\|=' on a 'bool' operand" }
  (void) ~a;					// { dg-error "bitwise '~' on a 'bool' operand" }
  return a & b;					// { dg-error "bitwise '&' on a 'bool' operand" }
}
template bool tf (bool, bool);
template int tf (int, int);

// The rule does not change which overload is chosen.
template <typename T>
auto pick (T t) -> decltype (t & t, 1) { return 1; }
long pick (...) { return 2; }
static_assert (sizeof (pick (true)) == sizeof (int));

[[subset::suppress(binary_op_on_bool)]];

void
again (bool a, bool b)
{
  a &= b;
}
