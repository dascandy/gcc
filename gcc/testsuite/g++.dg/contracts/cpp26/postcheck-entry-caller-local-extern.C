// -fcontract-postcheck-entry with caller-side checks: a call of a
// block-scope extern declaration is a call of the namespace-scope
// function it aliases once genericized, so the caller-side wrapper calls
// that function's postcheck entry point and the precondition is checked
// once.  This used to ICE in retarget_wrapper_call.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -fcontracts-client-check=pre -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

static int npre, npost;
static bool P (bool b) { ++npre; return b; }
static bool Q (bool b) { ++npost; return b; }
#define CHECK(expr, pre, post) \
  do { npre = npost = 0; expr; \
       if (npre != (pre) || npost != (post)) __builtin_abort (); } while (0)

int
use (int y)
{
  int loc (int x) pre (P (x > 0)) post (r: Q (r > 0));
  return loc (y);
}

int loc (int x) pre (P (x > 0)) post (r: Q (r > 0)) { return x; }

int
main ()
{
  CHECK (use (1), 1, 1);
}
