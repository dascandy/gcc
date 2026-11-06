// { dg-do compile { target c++17 } }
// A typedef or alias can be removed, and using it is an error when the
// policy is active, also through other aliases.

struct K { };
using RB [[policy::remove(core)]] = int;		// { dg-message "declared here" }
[[policy::remove(core, "use K")]] typedef K RC;
template <typename T> using RT [[policy::remove(core)]] = T;
struct S { using M [[policy::remove(core)]] = int; };
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
  [[policy::activate(core)]];
  RB b;			// { dg-error "'RB' \\\{aka 'int'\\\} was policy-removed" }
  RC c;			// { dg-error "'RC' \\\{aka 'K'\\\} was policy-removed: use K" }
  RT<int> t;		// { dg-error "'RT<int>' \\\{aka 'int'\\\} was policy-removed" }
  RB2 b2;		// { dg-error "'RB' \\\{aka 'int'\\\} was policy-removed" }
  RT2<long> t2;		// { dg-error "'RT<long int>' \\\{aka 'long int'\\\} was policy-removed" }
  S::M m;		// { dg-error "'S::M' \\\{aka 'int'\\\} was policy-removed" }
  RC {};		// { dg-error "'RC' \\\{aka 'K'\\\} was policy-removed" }
  (void) sizeof (RB);	// { dg-error "'RB' \\\{aka 'int'\\\} was policy-removed" }
  K k;
  int i;
}

// In a template, a dependent use is checked when it is instantiated, with
// the policies active at the definition of the template.
template <typename T>
void
inactive_template ()
{
  typename T::M m;
  RT<T> t;
}
[[policy::activate(core)]];
template <typename T>
void
active_template ()
{
  typename T::M m;	// { dg-error "'S::M' \\\{aka 'int'\\\} was policy-removed" }
  RT<T> t;		// { dg-error "'RT<S>' \\\{aka 'S'\\\} was policy-removed" }
}
[[policy::suppress(core)]];
void
use_templates ()
{
  [[policy::activate(core)]];
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
    [[policy::activate(core)]];
    static_assert (sizeof (q (1)) == sizeof (char));	// { dg-error "'RT<int>' \\\{aka 'int'\\\} was policy-removed" }
  }
}
