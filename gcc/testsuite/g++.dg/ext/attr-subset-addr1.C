// { dg-do compile { target c++17 } }
// Subset operator_unary_ampersand_overload disallows defining and using an
// overloaded unary operator&.  The built-in address-of operator, a binary
// operator& and std::addressof are unaffected, and so is a deleted overload,
// which cannot be used.

#include <memory>

struct A {};
struct B {};
struct D {};

// Defined before the subset is activated.
A *operator& (A &);				// { dg-message "declared here" }

[[subset::activate(operator_unary_ampersand_overload)]];

B *operator& (B &b) { return __builtin_addressof (b); }		// { dg-error "defining overloaded unary 'operator&'" }

struct M
{
  M *operator& () { return this; }		// { dg-error "defining overloaded unary 'operator&'" }
  const M *operator& () const;
  M operator& (int) { return *this; }
};
// The note for a use points to the definition.
const M *M::operator& () const { return this; }	// { dg-error "defining overloaded unary 'operator&'" }
// { dg-message "declared here" "" { target *-*-* } .-1 }

template <typename T>
struct W
{
  W *operator& () { return this; }		// { dg-error "defining overloaded unary 'operator&'" }
};

template <typename T>
T *operator& (T &, D);

// Deleting it is allowed.
struct NoAddr
{
  void operator& () const = delete;
};

void
use (A a, const M cm, M m, int i, NoAddr n)
{
  A *pa = &a;					// { dg-error "using overloaded unary 'operator&'" }
  pa = operator& (a);				// { dg-error "using overloaded unary 'operator&'" }
  const M *pm = &cm;				// { dg-error "using overloaded unary 'operator&'" }
  pm = cm.operator& ();				// { dg-error "using overloaded unary 'operator&'" }

  // The built-in operators, a binary operator& and std::addressof.
  int *pi = &i;
  i = i & 1;
  m = m & 1;
  pa = std::addressof (a);
  pm = std::addressof (m);
  const NoAddr *pn = std::addressof (n);
  void (*f) (A, const M, M, int, NoAddr) = &use;
  (void) pa; (void) pm; (void) pi; (void) pn; (void) f;
}

// In a template, a use is checked when it is instantiated.
template <typename T>
void
tuse (T t)
{
  (void) &t;					// { dg-error "using overloaded unary 'operator&'" }
  (void) std::addressof (t);
}
template void tuse (A);
template void tuse (int);

// The rule does not change which overload is chosen.
template <typename T>
auto pick (T t) -> decltype (&t, 1) { return 1; }
long pick (...) { return 2; }
static_assert (sizeof (pick (A ())) == sizeof (int));

[[subset::suppress(operator_unary_ampersand_overload)]];

D *operator& (D &d) { return std::addressof (d); }

void
again (A a)
{
  (void) &a;
}
