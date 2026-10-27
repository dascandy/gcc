// { dg-do compile { target c++20 } }
// Removing a class template has no effect on SFINAE: a substitution that
// uses it still succeeds.  The use is diagnosed only if the specialization
// that was formed with it is chosen while the subset is active.

template <typename T> struct [[subset::remove(core)]] CT { };
template <typename...> using void_t = void;

// Function templates.
template <typename T> auto sf (T) -> decltype (CT<T>{}, 1) { return 1; }	// { dg-message "required by the declaration of" }
long sf (...);

template <typename T> auto sg (T) -> decltype (CT<T>{}, 1);	// { dg-message "required by the declaration of" }
template <typename T> auto sg (T) -> decltype (T::nonexistent, 'c');
long sg (...);

template <typename T> auto sh (T) -> decltype (CT<T>{}, 1);
char sh (int);

// A partial specialization.
template <typename T, typename = void> struct has { static const int value = 1; };
template <typename T> struct has<T, void_t<decltype (CT<T>{})>> { static const int value = 2; };	// { dg-message "required by the partial specialization chosen for" }

// A requires-expression.
template <typename T> constexpr int req () { if constexpr (requires { CT<T>{}; }) return 1; else return 2; }

void
inactive ()
{
  static_assert (sizeof (sf ('a')) == sizeof (int));
  static_assert (sizeof (sg ('a')) == sizeof (int));
  static_assert (has<char>::value == 2);
  static_assert (req<char> () == 1);
}

void
active ()
{
  [[subset::activate(core)]];
  // The same candidates are chosen, and choosing one that uses CT is an
  // error.
  static_assert (sizeof (sf (1)) == sizeof (int));	// { dg-error "'CT<int>' was subset-removed" }
  static_assert (sizeof (sg (1)) == sizeof (int));	// { dg-error "'CT<int>' was subset-removed" }
  static_assert (has<int>::value == 2);			// { dg-error "'CT<int>' was subset-removed" }
  static_assert (req<long> () == 1);
  // Forming a candidate that uses CT is not an error if it is not chosen.
  static_assert (sizeof (sh (1)) == sizeof (char));
}

// A class template with a partial specialization: whether a specialization
// is removed is known when it is instantiated.
template <typename T> struct P { };
template <typename T> struct [[subset::remove(core)]] P<T *> { };

void
partial ()
{
  [[subset::activate(core)]];
  P<int> p1;
  P<int *> p2;		// { dg-error "'P<int\\*>' was subset-removed" }
  P<int **> *p3;	// { dg-error "'P<int\\*\\*>' was subset-removed" }
  P<int **> p4;		// { dg-error "'P<int\\*\\*>' was subset-removed" }
}

// Uses in a template are diagnosed when it is instantiated, using the
// subsets that were active at its definition.
[[subset::activate(core)]];
template <typename T> void body () { CT<T> c; CT<T> *p; }	// { dg-error "'CT<int>' was subset-removed" }
[[subset::suppress(core)]];
template <typename T> void body2 () { CT<T> c; }
void
use_body ()
{
  body<int> ();
  body2<char> ();
}
