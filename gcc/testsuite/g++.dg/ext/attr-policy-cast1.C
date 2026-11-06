// { dg-do compile { target c++17 } }
// Policies reinterpret_cast, const_cast and dynamic_cast each disallow the
// cast they are named after.  Other casts are unaffected.

struct B { virtual ~B (); };
struct D : B {};

void
inactive (B *b, const int *ci)
{
  (void) reinterpret_cast<long> (b);
  (void) const_cast<int *> (ci);
  (void) dynamic_cast<D *> (b);
}

[[policy::activate(reinterpret_cast)]];

void
reinterpret (B *b, const int *ci)
{
  (void) reinterpret_cast<long> (b);		// { dg-error "use of 'reinterpret_cast' in policy 'reinterpret_cast'" }
  (void) const_cast<int *> (ci);
  (void) dynamic_cast<D *> (b);
}

[[policy::suppress(reinterpret_cast), policy::activate(const_cast)]];

void
const_ (B *b, const int *ci)
{
  (void) reinterpret_cast<long> (b);
  (void) const_cast<int *> (ci);		// { dg-error "use of 'const_cast' in policy 'const_cast'" }
  (void) dynamic_cast<D *> (b);
}

[[policy::suppress(const_cast), policy::activate(dynamic_cast)]];

void
dynamic (B *b, const int *ci)
{
  (void) reinterpret_cast<long> (b);
  (void) const_cast<int *> (ci);
  (void) dynamic_cast<D *> (b);			// { dg-error "use of 'dynamic_cast' in policy 'dynamic_cast'" }
}

[[policy::suppress(dynamic_cast)]];

// A policy group defined with them, and the other casts: a C-style cast
// with the effect of one of them is disallowed as well (attr-policy-cast2.C
// has the details), and the rest are not.
[[policy::define(casts, reinterpret_cast, const_cast, dynamic_cast)]];
[[policy::activate(casts)]];

void
all (B *b, const int *ci, double d)
{
  (void) reinterpret_cast<long> (b);		// { dg-error "use of 'reinterpret_cast'" }
  (void) const_cast<int *> (ci);		// { dg-error "use of 'const_cast'" }
  (void) dynamic_cast<D *> (b);			// { dg-error "use of 'dynamic_cast'" }
  (void) static_cast<D *> (b);
  (void) (int *) ci;				// { dg-error "casts away qualifiers, as 'const_cast' does" }
  (void) (long) b;				// { dg-error "effect of 'reinterpret_cast'" }
  (void) (const int *) ci;
  (void) int (d);
  (void) __builtin_bit_cast (long, d);
}

// A template is checked where it is defined, even if never instantiated.
template <typename T>
T *
tf (const T *p)
{
  return const_cast<T *> (p);			// { dg-error "use of 'const_cast'" }
}

[[policy::suppress(casts)]];

// Removals can also be under these groups.
[[policy::remove(reinterpret_cast)]] void r ();	// { dg-message "declared here" }

void
use ()
{
  [[policy::activate(reinterpret_cast)]];
  r ();						// { dg-error "'void r\\(\\)' was policy-removed" }
}
