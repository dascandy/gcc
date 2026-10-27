// { dg-do compile { target c++17 } }
// Subset pointer_arithmetic disallows adding an integer to or subtracting one
// from a pointer, in any form, but allows subtracting two pointers and
// subscripting arrays.

#include <cstddef>

struct It
{
  int *p;
  It operator+ (int) const;
  It &operator+= (int);
  It &operator++ ();
  int &operator[] (int);
};

enum E { e1 = 1 };

struct ToInt { operator int () const { return 1; } };

void
inactive (int *p, int i)
{
  p = p + i;
  p += i;
  ++p;
  p[i] = 0;
}

[[subset::activate(pointer_arithmetic)]];

void
active (int *p, const int *q, int i, E e, It it, ToInt ti)
{
  int arr[4] = {};
  int *r;
  r = p + i;				// { dg-error "adding an integer to a pointer" }
  r = i + p;				// { dg-error "adding an integer to a pointer" }
  r = p - i;				// { dg-error "subtracting an integer from a pointer" }
  r = arr + 1;				// { dg-error "adding an integer to a pointer" }
  r = p + e;				// { dg-error "adding an integer to a pointer" }
  r = p + ti;				// { dg-error "adding an integer to a pointer" }
  p += i;				// { dg-error "adding an integer to a pointer" }
  p -= i;				// { dg-error "subtracting an integer from a pointer" }
  p += ti;				// { dg-error "adding an integer to a pointer" }
  ++p;					// { dg-error "incrementing a pointer" }
  p++;					// { dg-error "incrementing a pointer" }
  --p;					// { dg-error "decrementing a pointer" }
  p--;					// { dg-error "decrementing a pointer" }
  p[i] = 0;				// { dg-error "subscripting a pointer" }
  i[p] = 0;				// { dg-error "subscripting a pointer" }
  (void) sizeof (p + 1);		// { dg-error "adding an integer to a pointer" }
  (void) r;

  // Allowed.
  std::ptrdiff_t d = p - q;
  d = &arr[3] - arr;
  arr[i] = 0;
  i[arr] = 0;
  i = i + 1;
  ++i;
  bool b = p == q || p < q;
  it = it + 1;
  it += 1;
  ++it;
  it[1] = 0;
  for (int x : arr)
    (void) x;
  int *n = new int[4];
  delete[] n;
  (void) d; (void) b;
}

// In a template, it is checked when instantiated.
template <typename T>
T *
tf (T *p, int i)
{
  p[i] = T ();				// { dg-error "subscripting a pointer" }
  ++p;					// { dg-error "incrementing a pointer" }
  p += i;				// { dg-error "adding an integer to a pointer" }
  return p - i;				// { dg-error "subtracting an integer from a pointer" }
}

template <typename T>
std::ptrdiff_t
tdiff (T *p, T *q)
{
  T arr[2] = {};
  for (auto x : arr)
    (void) x;
  return p - q;
}

void
use (int *p)
{
  tf (p, 1);
  tdiff (p, p);
}

// The rule does not change which overload is chosen.
template <typename T>
auto sf (T p) -> decltype (p + 1, 1) { return 1; }
long sf (...) { return 2; }
static_assert (sizeof (sf ((int *) nullptr)) == sizeof (int));

[[subset::suppress(pointer_arithmetic)]];

void
again (int *p)
{
  ++p;
}
