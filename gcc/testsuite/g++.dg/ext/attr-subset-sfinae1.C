// { dg-do compile { target c++20 } }
// Activating a subset never changes which declaration overload resolution
// chooses, or whether a substitution fails.  It only makes the program
// ill-formed where a removed entity is used, or where a throw that the
// subset forbids is formed.

#include <exception>

[[subset::remove(core)]] char rf (int);
long rf (long);
struct [[subset::remove(core)]] RC { RC (int); };
struct K { [[subset::remove(core)]] K (int); K (long); };

// A removed function is still a candidate, and still the best one.
void
overload ()
{
  static_assert (sizeof (rf (1)) == sizeof (char));
  static_assert (sizeof (rf (1L)) == sizeof (long));
  K k1 (1);
  K k2 (1L);
  {
    [[subset::activate(core)]];
    static_assert (sizeof (rf (1)) == sizeof (char));	// { dg-error "'char rf\\(int\\)' was subset-removed" }
    static_assert (sizeof (rf (1L)) == sizeof (long));
    K k3 (1);						// { dg-error "'K::K\\(int\\)' was subset-removed" }
    K k4 (1L);
  }
}

// SFINAE: substitutions that use a removed entity succeed.  Choosing the
// resulting specialization is an error if the subset is active.
template <typename T> auto sfun (T t) -> decltype (rf (t), 'c');	// { dg-message "required by the declaration of" }
long sfun (...);
// A non-dependent use is checked where the template is defined.
template <typename T> auto scls (T t) -> decltype (RC (t), 'c');
long scls (...);
template <typename T, typename = decltype (rf (T ()))> char sdef (T);	// { dg-message "required by the declaration of" }
long sdef (...);

void
sfinae ()
{
  static_assert (sizeof (sfun (1)) == sizeof (char));
  static_assert (sizeof (scls (1)) == sizeof (char));
  static_assert (sizeof (scls ("x")) == sizeof (long));
  static_assert (sizeof (sdef (1)) == sizeof (char));
  {
    [[subset::activate(core)]];
    static_assert (sizeof (sfun (1)) == sizeof (char));	// { dg-error "'char rf\\(int\\)' was subset-removed" }
    static_assert (sizeof (sfun (1L)) == sizeof (char));
    static_assert (sizeof (scls (1)) == sizeof (char));
    static_assert (sizeof (scls ("x")) == sizeof (long));
    static_assert (sizeof (sdef (1)) == sizeof (char));	// { dg-error "'char rf\\(int\\)' was subset-removed" }
  }
}

// Constraints and requires-expressions only ask whether something is valid:
// they are satisfied the same way, and are never an error.
template <typename T> concept callable = requires (T t) { rf (t); RC (t); };
template <typename T> concept throwable = requires (T t) { throw t; };
template <callable T> char cf (T);
long cf (...);
template <throwable T> char tf (T);
long tf (...);
template <typename T> constexpr int
query ()
{
  if constexpr (requires (T t) { rf (t); throw t; })
    return 1;
  else
    return 2;
}

void
constraints ()
{
  [[subset::activate(core)]];
  [[subset::activate(throw_non_std_exception)]];
  static_assert (callable<int>);
  static_assert (sizeof (cf (1)) == sizeof (char));
  static_assert (throwable<int>);
  static_assert (sizeof (tf (1)) == sizeof (char));
  static_assert (query<int> () == 1);
}

// Traits ask whether something is valid, so they give the same answer and
// are not an error, though naming a removed class in one is.
void
traits ()
{
  [[subset::activate(core)]];
  static_assert (__is_constructible (K, int));
  static_assert (__is_convertible (int, K));
  static_assert (__is_constructible (RC, int));		// { dg-error "'RC' was subset-removed" }
}

// A throw that the subset forbids still forms a valid substitution.
template <typename T> auto sthrow (T t) -> decltype (throw t, 'c');
long sthrow (...);
void
throws ()
{
  [[subset::activate(throw_non_std_exception)]];
  static_assert (sizeof (sthrow (1)) == sizeof (char));
  static_assert (sizeof (sthrow (std::exception ())) == sizeof (char));
}
