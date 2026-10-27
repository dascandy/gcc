// { dg-do compile { target c++17 } }
// Subset operator_boolean_binary_overload disallows defining an overloaded
// operator&& or operator||, but not using one defined elsewhere.  The
// built-in operators are unaffected, and so is a deleted overload.

struct A {};
struct B {};
struct D {};

// Defined before the subset is activated.
bool operator&& (A, A);
bool operator|| (A, A) { return true; }

[[subset::activate(operator_boolean_binary_overload)]];

bool operator&& (B, B) { return true; }		// { dg-error "defining overloaded 'operator&&'" }
// { dg-message "always evaluates both of its operands" "" { target *-*-* } .-1 }
bool operator|| (B, B) { return true; }		// { dg-error "defining overloaded 'operator\\|\\|'" }
// { dg-message "always evaluates both of its operands" "" { target *-*-* } .-1 }

struct M
{
  bool operator&& (int) { return true; }	// { dg-error "defining overloaded 'operator&&'" }
  // { dg-message "always evaluates both" "" { target *-*-* } .-1 }
  bool operator|| (int);
  friend bool operator&& (int, M) { return true; }	// { dg-error "defining overloaded 'operator&&'" }
  // { dg-message "always evaluates both" "" { target *-*-* } .-1 }
};
bool M::operator|| (int) { return true; }	// { dg-error "defining overloaded 'operator\\|\\|'" }
// { dg-message "always evaluates both" "" { target *-*-* } .-1 }

template <typename T>
struct W
{
  bool operator|| (T) { return true; }		// { dg-error "defining overloaded 'operator\\|\\|'" }
  // { dg-message "always evaluates both" "" { target *-*-* } .-1 }
};

// Deleting one is allowed.
struct NoAnd
{
  void operator&& (NoAnd) = delete;
};

// Using one defined elsewhere is allowed, as are the built-in operators.
void
use (A a, M m, bool x, bool y)
{
  (void) (a && a);
  (void) (a || a);
  (void) operator|| (a, a);
  (void) (m && 1);
  (void) (x && y);
  (void) (x || y);
  W<int> w;
  (void) (w || 1);
}

[[subset::suppress(operator_boolean_binary_overload)]];

bool operator&& (D, D) { return true; }
