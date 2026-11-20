// -fcontract-postcheck-entry with caller-side checks: an argument of an
// empty class type is passed on by the caller-side wrapper as an
// EMPTY_CLASS_EXPR, which still forwards the wrapper's parameter, so the
// wrapper calls the postcheck entry point and each precondition is
// checked once.  This used to ICE in retarget_wrapper_call.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -fcontracts-client-check=pre -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

static int npre, npost;
static bool P (bool b) { ++npre; return b; }
static bool Q (bool b) { ++npost; return b; }
#define CHECK(expr, pre, post) \
  do { npre = npost = 0; expr; \
       if (npre != (pre) || npost != (post)) __builtin_abort (); } while (0)

struct E {};
struct HasIt { bool ok () const { return true; } };

int f (E, int x) pre (P (x > 0)) post (r: Q (r > 0)) { return x; }
int g (int x, E) pre (P (x > 0)) { return x; }
int h (E e) pre (P (true)) { return 1; }
template <class T> int t (T v) pre (P (v.ok ())) { return 1; }
struct F { friend int fr (F, int x) pre (P (x > 0)) { return x; } };

int
main ()
{
  CHECK (f (E{}, 1), 1, 1);
  CHECK (g (1, E{}), 1, 0);
  CHECK (h (E{}), 1, 0);
  CHECK (t (HasIt{}), 1, 0);
  CHECK (fr (F{}, 1), 1, 0);
}
