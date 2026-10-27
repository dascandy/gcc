// { dg-do compile { target c++17 } }
// A typedef or alias can be removed, and using it is an error when the
// subset is active, also through other aliases.

struct K { };
using RB [[subset::remove(core)]] = int;		// { dg-message "declared here" }
[[subset::remove(core, "use K")]] typedef K RC;
template <typename T> using RT [[subset::remove(core)]] = T;
struct S { using M [[subset::remove(core)]] = int; };
using RB2 = RB;
template <typename T> using RT2 = RT<T>;

void
inactive ()
{
  RB b;
  RC c;
  RT<int> t;
  RB2 b2;
  S::M m;
}

void
active ()
{
  [[subset::activate(core)]];
  RB b;			// { dg-error "'RB' \\\{aka 'int'\\\} was subset-removed" }
  RC c;			// { dg-error "'RC' \\\{aka 'K'\\\} was subset-removed: use K" }
  RT<int> t;		// { dg-error "'RT<int>' \\\{aka 'int'\\\} was subset-removed" }
  RB2 b2;		// { dg-error "'RB' \\\{aka 'int'\\\} was subset-removed" }
  RT2<long> t2;		// { dg-error "'RT<long int>' \\\{aka 'long int'\\\} was subset-removed" }
  S::M m;		// { dg-error "'S::M' \\\{aka 'int'\\\} was subset-removed" }
  RC {};		// { dg-error "'RC' \\\{aka 'K'\\\} was subset-removed" }
  (void) sizeof (RB);	// { dg-error "'RB' \\\{aka 'int'\\\} was subset-removed" }
  K k;
  int i;
}

// In a template, a dependent use is checked when it is instantiated, with
// the subsets active at the definition of the template.
template <typename T>
void
inactive_template ()
{
  typename T::M m;
  RT<T> t;
}
[[subset::activate(core)]];
template <typename T>
void
active_template ()
{
  typename T::M m;	// { dg-error "'S::M' \\\{aka 'int'\\\} was subset-removed" }
  RT<T> t;		// { dg-error "'RT<S>' \\\{aka 'S'\\\} was subset-removed" }
}
[[subset::suppress(core)]];
void
use_templates ()
{
  [[subset::activate(core)]];
  inactive_template<S> ();
  active_template<S> ();
}

// A substitution through a removed alias template still succeeds, and is
// diagnosed if it is chosen.
template <typename T> auto q (T) -> decltype (RT<T> {}, 'c');	// { dg-message "required by the declaration of" }
long q (...);
void
sfinae ()
{
  static_assert (sizeof (q (1)) == sizeof (char));
  {
    [[subset::activate(core)]];
    static_assert (sizeof (q (1)) == sizeof (char));	// { dg-error "'RT<int>' \\\{aka 'int'\\\} was subset-removed" }
  }
}
