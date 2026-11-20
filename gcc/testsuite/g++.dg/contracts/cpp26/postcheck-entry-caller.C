// -fcontract-postcheck-entry with caller-side checks: a caller that has
// enforced every precondition calls the postcheck entry point, so each
// precondition is checked once, at the call.  A call through a pointer is
// not checked at the call and goes to the usual entry point.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -fcontracts-client-check=pre -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <cstdarg>

static int npre, npost;
static bool P (bool b) { ++npre; return b; }
static bool Q (bool b) { ++npost; return b; }
#define CHECK(expr, pre, post) \
  do { npre = npost = 0; expr; \
       if (npre != (pre) || npost != (post)) __builtin_abort (); } while (0)

struct S { int v; S (int v) : v (v) {} S (const S &o) : v (o.v) {} ~S () {} };

__attribute__((noinline)) int f (int x) pre (P (x > 0)) post (r: Q (r > 1))
{ return x + 1; }
S g (const S s) pre (P (s.v > 0)) post (r: Q (r.v == s.v)) { return s; }
struct C
{
  int m;
  C (const int x) pre (P (x > 0)) post (Q (m == x)) : m (x) {}
  ~C () pre (P (m > 0)) {}
};
inline int il (int x) pre (P (x > 0)) { return x; }
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

int main ()
{
  CHECK (f (1), 1, 1);
  int (*fp) (int) = f;
  CHECK (fp (1), 1, 1);
  CHECK ({ S r = g (S (2)); if (r.v != 2) __builtin_abort (); }, 1, 1);
  CHECK ({ C c (3); }, 2, 1);
  CHECK (il (4), 1, 0);
  CHECK (if (va (2, 1, 2) != 3) __builtin_abort (), 1, 0);
}
