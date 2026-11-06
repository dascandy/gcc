// { dg-do compile { target c++17 } }
// Policies reinterpret_cast and const_cast also disallow a C-style cast, in
// cast notation or functional notation, with the effect of the cast they are
// named after: one that [expr.cast] makes a reinterpret_cast, and one that
// casts away qualifiers.  A C-style cast that is a const_cast by
// [expr.cast] but only adds qualifiers, or changes nothing, is allowed, as
// is a static_cast.  A C-style cast is never a dynamic_cast, so policy
// dynamic_cast does not apply to one, not even to a downcast.

struct B { virtual ~B (); };
struct D : B {};
struct U {};
struct M { int i; };
void fn ();
typedef long L;

void
inactive (int *p, const int *cp)
{
  (void) (long) p;
  (void) (int *) cp;
}

[[policy::activate(reinterpret_cast)]];

void
reinterpret (int *p, const int *cp, B *b, int arr[2], int M::*pm, float &f)
{
  (void) (long) p;		// { dg-error "cast with the effect of 'reinterpret_cast' in policy 'reinterpret_cast'" }
  (void) L (p);			// { dg-error "effect of 'reinterpret_cast'" }
  (void) (U *) b;		// { dg-error "effect of 'reinterpret_cast'" }
  (void) (void (*) (int)) fn;	// { dg-error "effect of 'reinterpret_cast'" }
  (void) (long M::*) pm;	// { dg-error "effect of 'reinterpret_cast'" }
  (void) (int &) f;		// { dg-error "effect of 'reinterpret_cast'" }
  (void) (char *) cp;		// { dg-error "effect of 'reinterpret_cast'" }
  (void) (int *) 4096L;		// { dg-error "effect of 'reinterpret_cast'" }

  (void) (D *) b;
  (void) (B *) (D *) b;
  (void) (void *) p;
  (void) (int *) (void *) p;
  (void) (const int *) p;
  (void) (int *) p;
  (void) (int *) arr;
  (void) (long) 1.5;
  (void) L (1.5);
  (void) (bool) p;
  (void) (U) U ();
}

[[policy::suppress(reinterpret_cast), policy::activate(const_cast)]];

void
const_ (int *p, const int *cp, const B *cb, const char *const *ccpp,
	const int &cr, volatile int *vp, const int carr[2],
	const volatile int *cvp)
{
  (void) (int *) cp;		// { dg-error "cast that casts away qualifiers, as 'const_cast' does, in policy 'const_cast'" }
  (void) (int &) cr;		// { dg-error "casts away qualifiers" }
  (void) (int &&) cr;		// { dg-error "casts away qualifiers" }
  (void) (D *) cb;		// { dg-error "casts away qualifiers" }
  (void) (char *) cp;		// { dg-error "casts away qualifiers" }
  (void) (char **) ccpp;	// { dg-error "casts away qualifiers" }
  (void) (int *) vp;		// { dg-error "casts away qualifiers" }
  (void) (int *) carr;		// { dg-error "casts away qualifiers" }
  (void) (const int *) cvp;	// { dg-error "casts away qualifiers" }

  (void) (const int *) p;
  (void) (const volatile int *) cp;
  (void) (const char *const *) ccpp;
  (void) (const D *) cb;
  (void) (int) cr;
  (void) (long) p;
}

// A cast with the effect of both gets both errors.
[[policy::activate(reinterpret_cast)]];

void
both (const int *cp)
{
  (void) (char *) cp;		// { dg-error "effect of 'reinterpret_cast'" }
  // { dg-error "casts away qualifiers" "" { target *-*-* } .-1 }
}

[[policy::suppress(reinterpret_cast), policy::suppress(const_cast)]];
[[policy::activate(dynamic_cast)]];

void
dynamic (B *b, const B *cb)
{
  (void) (D *) b;
  (void) (const D *) cb;
}
