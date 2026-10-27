// { dg-do compile { target c++17 } }
// Subset operator_comma_overload disallows defining and using an overloaded
// comma operator.  The built-in comma operator is unaffected, and so is a
// deleted overload, which cannot be used.

struct A {};
struct B {};
struct D {};

// Defined before the subset is activated.
A operator, (A, A);				// { dg-message "declared here" }

[[subset::activate(operator_comma_overload)]];

B operator, (B, B) { return B (); }		// { dg-error "defining overloaded 'operator,'" }

struct M
{
  M operator, (int) { return *this; }		// { dg-error "defining overloaded 'operator,'" }
  M operator, (long);
  friend M operator, (int, M) { return M (); }	// { dg-error "defining overloaded 'operator,'" }
};
// The note for a use points to the definition.
M M::operator, (long) { return *this; }		// { dg-error "defining overloaded 'operator,'" }
// { dg-message "declared here" "" { target *-*-* } .-1 }

template <typename T>
struct W
{
  W operator, (T) { return *this; }		// { dg-error "defining overloaded 'operator,'" }
};

template <typename T>
T operator, (T, D) { return T (); }		// { dg-error "defining overloaded 'operator,'" }

// Deleting it is allowed.
struct NoComma
{
  void operator, (NoComma) = delete;
};

void
use (A a, M m, int i)
{
  a, a;						// { dg-error "using overloaded 'operator,'" }
  operator, (a, a);				// { dg-error "using overloaded 'operator,'" }
  m, 1L;					// { dg-error "using overloaded 'operator,'" }
  m.operator, (1L);				// { dg-error "using overloaded 'operator,'" }

  // The built-in comma operator.
  i = (i, 2);
  for (int j = 0, k = 0; j < 2; ++j, ++k)
    ;
  (void) a, a;
  void (), m;
  decltype (void (), 0) x = 0;
  (void) x;
}

// In a template, a use is checked when it is instantiated.
template <typename T>
void
tuse (T t)
{
  t, t;						// { dg-error "using overloaded 'operator,'" }
  (void) t, t;
}
template void tuse (A);
template void tuse (int);

// The rule does not change which overload is chosen.
template <typename T>
auto pick (T t) -> decltype (t, t, 1) { return 1; }
long pick (...) { return 2; }
static_assert (sizeof (pick (A ())) == sizeof (int));

[[subset::suppress(operator_comma_overload)]];

D operator, (D, D) { return D (); }

void
again (A a)
{
  a, a;
}
