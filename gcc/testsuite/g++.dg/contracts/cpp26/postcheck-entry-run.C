// -fcontract-postcheck-entry: each function with a precondition has two
// entry points, and whichever a call goes through, the preconditions and
// postconditions are each checked once: free functions, a class passed and
// returned by value, a named return value, noexcept, a C-variadic
// function, inline and template functions, members, a lambda, constructors
// and destructors, and calls through function pointers.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <cstdarg>

static int npre, npost;
static bool P (bool b) { ++npre; return b; }
static bool Q (bool b) { ++npost; return b; }
#define CHECK(expr, pre, post) \
  do { npre = npost = 0; expr; \
       if (npre != (pre) || npost != (post)) __builtin_abort (); } while (0)

struct S
{
  int v;
  S (int v) : v (v) {}
  S (const S &o) : v (o.v) {}
  ~S () { v = -1; }
};

int f (int x) pre (P (x > 0)) post (r: Q (r > 1)) { return x + 1; }
S g (const S s, const int k) pre (P (s.v > 0)) post (r: Q (r.v == s.v + k))
{ return S (s.v + k); }
S nrv (const int k) pre (P (k > 0)) post (r: Q (r.v == k)) { S s (k); return s; }
int h (const int x) noexcept pre (P (x > 0)) post (r: Q (r == x)) { return x; }
inline int il (int x) pre (P (x != 0)) { return x * 3; }
template<class T> T tf (const T x) pre (P (x > 0)) post (r: Q (r == x))
{ return x; }
int va (int n, ...) pre (P (n >= 0))
{
  va_list ap;
  va_start (ap, n);
  int s = 0;
  for (int i = 0; i < n; ++i)
    s += va_arg (ap, int);
  va_end (ap);
  return s;
}

struct C
{
  int m;
  C (const int x) pre (P (x > 0)) post (Q (m == x)) : m (x) {}
  ~C () pre (P (m > 0)) {}
  int get (const int k) const pre (P (k >= 0)) post (r: Q (r == m + k))
  { return m + k; }
  static int st (int k) pre (P (k > 0)) { return k; }
};

struct VB { int b = 1; };
struct D : virtual VB
{
  int m;
  D (const int x) pre (P (x > 0)) post (Q (m == x));
  ~D () pre (P (m > 0));
};
D::D (const int x) : m (x) {}
D::~D () {}
struct E : D { E () : D (2) {} };

int main ()
{
  CHECK (if (f (1) != 2) __builtin_abort (), 1, 1);
  int (*fp) (int) = f;
  CHECK (if (fp (2) != 3) __builtin_abort (), 1, 1);
  CHECK ({ S r = g (S (3), 4); if (r.v != 7) __builtin_abort (); }, 1, 1);
  CHECK ({ S r = nrv (9); if (r.v != 9) __builtin_abort (); }, 1, 1);
  CHECK (h (5), 1, 1);
  CHECK (il (2), 1, 0);
  CHECK (tf (3), 1, 1);
  CHECK (tf (3.0), 1, 1);
  CHECK (if (va (3, 1, 2, 3) != 6) __builtin_abort (), 1, 0);
  CHECK ({ C c (4); }, 2, 1);
  CHECK ({ C *c = new C (4); delete c; }, 2, 1);
  CHECK ({ C c (4); if (c.get (1) != 5) __builtin_abort (); }, 3, 2);
  int (C::*mp) (int) const = &C::get;
  CHECK ({ C c (4); if ((c.*mp) (1) != 5) __builtin_abort (); }, 3, 2);
  CHECK (C::st (1), 1, 0);
  CHECK ({ D d (4); }, 2, 1);
  CHECK ({ E e; }, 2, 1);
  auto lam = [] (int x) pre (P (x > 0)) { return x; };
  CHECK (lam (1), 1, 0);
  int (*lp) (int) = lam;
  CHECK (lp (1), 1, 0);
}
