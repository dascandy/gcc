// { dg-do compile { target c++17 } }
// Subsets reinterpret_cast, const_cast and dynamic_cast each disallow the
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

[[subset::activate(reinterpret_cast)]];

void
reinterpret (B *b, const int *ci)
{
  (void) reinterpret_cast<long> (b);		// { dg-error "use of 'reinterpret_cast' in subset 'reinterpret_cast'" }
  (void) const_cast<int *> (ci);
  (void) dynamic_cast<D *> (b);
}

[[subset::suppress(reinterpret_cast), subset::activate(const_cast)]];

void
const_ (B *b, const int *ci)
{
  (void) reinterpret_cast<long> (b);
  (void) const_cast<int *> (ci);		// { dg-error "use of 'const_cast' in subset 'const_cast'" }
  (void) dynamic_cast<D *> (b);
}

[[subset::suppress(const_cast), subset::activate(dynamic_cast)]];

void
dynamic (B *b, const int *ci)
{
  (void) reinterpret_cast<long> (b);
  (void) const_cast<int *> (ci);
  (void) dynamic_cast<D *> (b);			// { dg-error "use of 'dynamic_cast' in subset 'dynamic_cast'" }
}

[[subset::suppress(dynamic_cast)]];

// A subset group defined with them, and the other casts.
[[subset::define(casts, reinterpret_cast, const_cast, dynamic_cast)]];
[[subset::activate(casts)]];

void
all (B *b, const int *ci, double d)
{
  (void) reinterpret_cast<long> (b);		// { dg-error "use of 'reinterpret_cast'" }
  (void) const_cast<int *> (ci);		// { dg-error "use of 'const_cast'" }
  (void) dynamic_cast<D *> (b);			// { dg-error "use of 'dynamic_cast'" }
  (void) static_cast<D *> (b);
  (void) (int *) ci;
  (void) (long) b;
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

[[subset::suppress(casts)]];

// Removals can also be under these groups.
[[subset::remove(reinterpret_cast)]] void r ();	// { dg-message "declared here" }

void
use ()
{
  [[subset::activate(reinterpret_cast)]];
  r ();						// { dg-error "'void r\\(\\)' was subset-removed" }
}
