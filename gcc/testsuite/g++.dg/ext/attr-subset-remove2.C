// { dg-do compile { target c++17 } }
// Using or instantiating a [[subset::remove(core)]] entity is an error when
// subset core is active, with the reason if it has one.

// Function.
[[subset::remove(core)]] void f ();
[[subset::remove(other)]] void o ();
void f2 () {}

// Function template, and an explicit specialization of a kept one.
template <typename T> [[subset::remove(core)]] void ft (T) {}
template <typename T> void ft2 (T) {}
template <> [[subset::remove(core)]] void ft2<int> (int) {}

// Class.
struct [[subset::remove(core)]] C { int i; };

// Member functions.
struct M {
  [[subset::remove(core)]] void mf ();
  [[subset::remove(core)]] static void sf () {}
  template <typename T> [[subset::remove(core)]] void mt (T) {}
  void ok () {}
};

// Class template.
template <typename T> struct [[subset::remove(core)]] CT { T t; };

// Partial specialization.
template <typename T> struct P { T t; };
template <typename T> struct [[subset::remove(core)]] P<T *> { T *t; };

// Explicit specialization.
template <typename T> struct E { T t; };
template <> struct [[subset::remove(core)]] E<int> { int t; };

// Variable and variable template.
[[subset::remove(core)]] int v;
[[subset::remove(core)]] extern int ev;
template <typename T> [[subset::remove(core)]] T vt = T ();

void
use ()
{
  [[subset::activate(core)]];
  f ();			// { dg-error "'void f\\(\\)' was subset-removed" }
  f2 ();
  ft (1);		// { dg-error "'void ft\\(T\\) \\\[with T = int\\\]' was subset-removed" }
  ft2 (1.0);
  ft2 (1);		// { dg-error "'void ft2\\(T\\) \\\[with T = int\\\]' was subset-removed" }
  C c;			// { dg-error "'C' was subset-removed" }
  M m;
  m.mf ();		// { dg-error "'void M::mf\\(\\)' was subset-removed" }
  M::sf ();		// { dg-error "'static void M::sf\\(\\)' was subset-removed" }
  m.mt (1);		// { dg-error "'void M::mt\\(T\\) \\\[with T = int\\\]' was subset-removed" }
  m.ok ();
  CT<int> ct;		// { dg-error "'CT<int>' was subset-removed" }
  P<int> p1;
  P<int *> p2;		// { dg-error "'P<int\\*>' was subset-removed" }
  E<long> e1;
  E<int> e2;		// { dg-error "'E<int>' was subset-removed" }
  v = 1;		// { dg-error "'v' was subset-removed" }
  ev = 1;		// { dg-error "'ev' was subset-removed" }
  vt<int> = 1;		// { dg-error "'vt<int>' was subset-removed" }
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
  ev = 2;
  vt<char> = 2;
  {
    [[subset::activate(other)]];
    f ();
  }
}

// A block restores the active subsets on exit.
void
use_block ()
{
  {
    [[subset::activate(core)]];
    f ();		// { dg-error "'void f\\(\\)' was subset-removed" }
  }
  f ();
}

// Removed in several groups: an error if any of them is active.
[[subset::remove(core), subset::remove(extra)]] void g ();

// A template uses the subsets that were active at its definition.
[[subset::activate(extra)]];
template <typename T> void t1 () { g (); }	// { dg-error "'void g\\(\\)' was subset-removed" }
[[subset::suppress(extra)]];
template <typename T> void t2 () { g (); }
void use_templates () { t1<int> (); t2<int> (); }

// Reasons.
[[subset::remove(core, "use h2 instead")]] void h ();	// { dg-message "declared here" }
struct [[subset::remove(core, "use R2")]] R {};	// { dg-message "declared here" }
template <typename T> struct [[subset::remove(core, "use RT2")]] RT {};	// { dg-message "declared here" }
void
use_reasons ()
{
  [[subset::activate(core)]];
  h ();			// { dg-error "'void h\\(\\)' was subset-removed: use h2 instead" }
  R r;			// { dg-error "'R' was subset-removed: use R2" }
  RT<int> rt;		// { dg-error "'RT<int>' was subset-removed: use RT2" }
}
