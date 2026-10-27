// { dg-do compile { target c++17 } }
// Subset no_wchar_t disallows uses of the type wchar_t, and wide character
// and string literals, which have that type.

#include <cstddef>

void
inactive ()
{
  wchar_t c = L'x';
  const wchar_t *s = L"abc";
  (void) c; (void) s;
}

[[subset::activate(no_wchar_t)]];

void f (wchar_t);				// { dg-error "use of 'wchar_t' in subset 'no_wchar_t'" }
wchar_t g ();					// { dg-error "use of 'wchar_t'" }
void h (const wchar_t *s) { (void) s; }		// { dg-error "use of 'wchar_t'" }

struct S
{
  wchar_t member;				// { dg-error "use of 'wchar_t'" }
};

template <typename T> struct W {};
W<wchar_t> wt;					// { dg-error "use of 'wchar_t'" }

typedef wchar_t wide;				// { dg-error "use of 'wchar_t'" }

std::size_t operator""_n (const wchar_t *, std::size_t); // { dg-error "use of 'wchar_t'" }
int operator""_c (wchar_t);			// { dg-error "use of 'wchar_t'" }

void
active (int i)
{
  wchar_t c;					// { dg-error "use of 'wchar_t'" }
  int a (wchar_t (1));				// { dg-error "use of 'wchar_t'" }
  i = sizeof (wchar_t);				// { dg-error "use of 'wchar_t'" }
  i = (wchar_t) i;				// { dg-error "use of 'wchar_t'" }
  i = static_cast<wchar_t> (i);			// { dg-error "use of 'wchar_t'" }
  (void) L'x';					// { dg-error "wide character literal in subset 'no_wchar_t'" }
  (void) L"abc";				// { dg-error "wide string literal in subset 'no_wchar_t'" }
  (void) L"ab" "cd";				// { dg-error "wide string literal" }
  (void) "ab" L"cd";				// { dg-error "wide string literal" }
  (void) L"ab"_n;				// { dg-error "wide string literal" }
  (void) L'x'_c;				// { dg-error "wide character literal" }

  // Allowed: other character types and their literals.
  char n = 'x';
  char16_t u = u'x';
  char32_t U = U'x';
  const char *ns = "abc";
  const char16_t *us = u"abc";
  (void) c; (void) a; (void) n; (void) u; (void) U; (void) ns; (void) us;
}

// A template is checked where it is defined, even if never instantiated.
template <typename T>
void
tf ()
{
  T t = L'x';					// { dg-error "wide character literal" }
  (void) t;
}

[[subset::suppress(no_wchar_t)]];

wchar_t again = L'y';
