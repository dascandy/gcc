// { dg-do compile { target c++17 } }
// Using a removed class through a typedef or an alias is a use of the
// class.

struct [[subset::remove(core)]] C { };
template <typename T> struct [[subset::remove(core)]] CT { };

// Declared while the subset is not active.
using A = C;
typedef C TD;
using A2 = A;
template <typename T> using AT = CT<T>;
template <typename T> using AC = C;
template <typename T> using AT2 = AT<T>;
struct S { using M = C; };

void
active ()
{
  [[subset::activate(core)]];
  A a;			// { dg-error "'C' was subset-removed" }
  TD td;		// { dg-error "'C' was subset-removed" }
  A2 a2;		// { dg-error "'C' was subset-removed" }
  A *p;			// { dg-error "'C' was subset-removed" }
  A {};			// { dg-error "'C' was subset-removed" }
  (void) sizeof (A);	// { dg-error "'C' was subset-removed" }
  delete new A;		// { dg-error "'C' was subset-removed" }
  AT<int> at;		// { dg-error "'CT<int>' was subset-removed" }
  AT<int> *pat;		// { dg-error "'CT<int>' was subset-removed" }
  AT2<char> at2;	// { dg-error "'CT<char>' was subset-removed" }
  AC<int> ac;		// { dg-error "'C' was subset-removed" }
  S::M m;		// { dg-error "'C' was subset-removed" }
}

void
inactive ()
{
  A a;
  AT<long> at;
  AT2<long> at2;
  S::M m;
}

// In a template, a dependent use is checked when it is instantiated.
[[subset::activate(core)]];
template <typename T>
void
dependent ()
{
  AT<T> x;		// { dg-error "'CT<S>' was subset-removed" }
  typename T::M m;	// { dg-error "'C' was subset-removed" }
}
[[subset::suppress(core)]];
void use_dependent () { dependent<S> (); }

// A substitution through an alias template still succeeds, and is
// diagnosed if it is chosen.
template <typename T> auto h (T) -> decltype (AT<T> {}, 'c');	// { dg-message "required by the declaration of" }
long h (...);
void
sfinae ()
{
  static_assert (sizeof (h (1)) == sizeof (char));
  {
    [[subset::activate(core)]];
    static_assert (sizeof (h (1)) == sizeof (char));	// { dg-error "'CT<int>' was subset-removed" }
  }
}
