// { dg-do compile { target c++17 } }
// Using or instantiating a [[policy::remove(core)]] entity is an error when
// policy core is active, with the reason if it has one.

// Function.
[[policy::remove(core)]] void f ();
[[policy::remove(other)]] void o ();
void f2 () {}

// Function template, and an explicit specialization of a kept one.
template <typename T> [[policy::remove(core)]] void ft (T) {}
template <typename T> void ft2 (T) {}
template <> [[policy::remove(core)]] void ft2<int> (int) {}

// Class.
struct [[policy::remove(core)]] C { int i; };

// Member functions.
struct M {
  [[policy::remove(core)]] void mf ();
  [[policy::remove(core)]] static void sf () {}
  template <typename T> [[policy::remove(core)]] void mt (T) {}
  void ok () {}
};

// Class template.
template <typename T> struct [[policy::remove(core)]] CT { T t; };

// Partial specialization.
template <typename T> struct P { T t; };
template <typename T> struct [[policy::remove(core)]] P<T *> { T *t; };

// Explicit specialization.
template <typename T> struct E { T t; };
template <> struct [[policy::remove(core)]] E<int> { int t; };

// Variable and variable template.
[[policy::remove(core)]] int v;
template <typename T> [[policy::remove(core)]] T vt = T ();

void
use ()
{
  [[policy::activate(core)]];
  f ();			// { dg-error "'void f\\(\\)' was policy-removed" }
  f2 ();
  ft (1);		// { dg-error "'void ft\\(T\\) \\\[with T = int\\\]' was policy-removed" }
  ft2 (1.0);
  ft2 (1);		// { dg-error "'void ft2\\(T\\) \\\[with T = int\\\]' was policy-removed" }
  C c;			// { dg-error "'C' was policy-removed" }
  M m;
  m.mf ();		// { dg-error "'void M::mf\\(\\)' was policy-removed" }
  M::sf ();		// { dg-error "'static void M::sf\\(\\)' was policy-removed" }
  m.mt (1);		// { dg-error "'void M::mt\\(T\\) \\\[with T = int\\\]' was policy-removed" }
  m.ok ();
  CT<int> ct;		// { dg-error "'CT<int>' was policy-removed" }
  P<int> p1;
  P<int *> p2;		// { dg-error "'P<int\\*>' was policy-removed" }
  E<long> e1;
  E<int> e2;		// { dg-error "'E<int>' was policy-removed" }
  v = 1;		// { dg-error "'v' was policy-removed" }
  vt<int> = 1;		// { dg-error "'vt<int>' was policy-removed" }
}

// Not active: no errors.
void
use_inactive ()
{
  f ();
  ft (2);
  ft2 (2);
  C c;
  M m;
  m.mf ();
  CT<int> ct1;
  CT<char> ct2;
  P<char *> p;
  E<int> e;
  v = 2;
  vt<char> = 2;
  {
    [[policy::activate(other)]];
    f ();
  }
}

// A block restores the active policies on exit.
void
use_block ()
{
  {
    [[policy::activate(core)]];
    f ();		// { dg-error "'void f\\(\\)' was policy-removed" }
  }
  f ();
}

// Removed in several groups: an error if any of them is active.
[[policy::remove(core), policy::remove(extra)]] void g ();

// A template uses the policies that were active at its definition.
[[policy::activate(extra)]];
template <typename T> void t1 () { g (); }	// { dg-error "'void g\\(\\)' was policy-removed" }
[[policy::suppress(extra)]];
template <typename T> void t2 () { g (); }
void use_templates () { t1<int> (); t2<int> (); }

// Reasons.
[[policy::remove(core, "use h2 instead")]] void h ();	// { dg-message "declared here" }
struct [[policy::remove(core, "use R2")]] R {};	// { dg-message "declared here" }
template <typename T> struct [[policy::remove(core, "use RT2")]] RT {};	// { dg-message "declared here" }
void
use_reasons ()
{
  [[policy::activate(core)]];
  h ();			// { dg-error "'void h\\(\\)' was policy-removed: use h2 instead" }
  R r;			// { dg-error "'R' was policy-removed: use R2" }
  RT<int> rt;		// { dg-error "'RT<int>' was policy-removed: use RT2" }
}
