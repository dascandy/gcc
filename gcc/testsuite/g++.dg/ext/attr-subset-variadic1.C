// { dg-do compile { target c++20 } }
// Subset variadic_functions disallows calls that pass arguments to the
// ellipsis of a C-style variadic function.  Declaring and defining such
// functions stays allowed, as do calls that pass nothing to the ellipsis and
// calls in unevaluated operands, where the ellipsis ranks an overload last.

#include <cstdarg>
#include <type_traits>

[[subset::activate(variadic_functions)]];

int log (const char *, ...);			// { dg-message "declared here" }
int none (...);					// { dg-message "declared here" }

// Defining one stays allowed.
int
sum (int n, ...)				// { dg-message "declared here" }
{
  va_list ap;
  va_start (ap, n);
  int s = 0;
  for (int i = 0; i < n; i++)
    s += va_arg (ap, int);
  va_end (ap);
  return s;
}

struct S
{
  S (int, ...);					// { dg-message "declared here" }
  void m (...);					// { dg-message "declared here" }
};

template <typename T>
void
tv (T, ...)					// { dg-message "declared here" }
{
}

// A variadic template is not a C-style variadic function.
template <typename... Ts>
void pack (Ts...) {}

void
calls (int (*fp) (int, ...))
{
  log ("x");
  log ("%d", 1);				// { dg-error "passing an argument to the '...' of 'int log" }
  log ("%d %d", 1, 2);				// { dg-error "passing an argument to the '...'" }
  none ();
  none (1);					// { dg-error "passing an argument to the '...' of 'int none" }
  sum (0);
  sum (1, 1);					// { dg-error "passing an argument to the '...' of 'int sum" }
  fp (1);
  fp (1, 2);					// { dg-error "passing an argument to the '...' of a function" }
  S s (1);
  S s2 (1, 2);					// { dg-error "passing an argument to the '...' of 'S::S" }
  s.m ();
  s.m (1);					// { dg-error "passing an argument to the '...' of 'void S::m" }
  tv (1);
  tv (1, 2);					// { dg-error "passing an argument to the '...' of 'void tv" }
  pack (1, 2, 3);
}

// In a template, a call is checked when it is instantiated.
template <typename T>
void
tcall (T t)
{
  sum (1, t);					// { dg-error "passing an argument to the '...' of 'int sum" }
}
template void tcall (int);

// The ellipsis as the lowest ranked overload keeps working in unevaluated
// operands.
template <typename T>
struct has_size
{
  template <typename U>
  static auto test (int) -> decltype (std::declval<U> ().size (), char ());
  template <typename U>
  static long test (...);
  static constexpr bool value = sizeof (test<T> (0)) == sizeof (char);
};
struct Sized { int size (); };
static_assert (has_size<Sized>::value);
static_assert (!has_size<int>::value);

template <typename T>
auto rank (T t, int) -> decltype (t.size (), std::true_type ());
template <typename T>
std::false_type rank (T, ...);
static_assert (decltype (rank (Sized (), 0))::value);
static_assert (!decltype (rank (1, 0))::value);
static_assert (noexcept (none (1)) == false);

// The rule does not change which overload is chosen.
template <typename T>
auto pick (T t) -> decltype (log ("", t), 1) { return 1; }
long pick (...) { return 2; }
static_assert (sizeof (pick (1)) == sizeof (int));

[[subset::suppress(variadic_functions)]];

void
again ()
{
  log ("%d", 1);
}
