// -fcontract-postcheck-entry with caller-side checks: a constant
// evaluation that calls a constexpr function template through its
// caller-side wrapper defines the wrapper, and so declares the postcheck
// entry point, before the specialization is instantiated.  The postcheck
// entry point then gets the parameters and result of the instantiation.
// This used to ICE in start_postcheck_function.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -fcontracts-client-check=pre -fcontract-evaluation-semantic=enforce" }

template <class T>
constexpr T f (const T x) pre (x > 0) post (r: r == x + 1) { return x + 1; }

constexpr int y = f (1);
static_assert (y == 2);

struct S { int v; constexpr S (int v) : v (v) {} };
template <class T>
constexpr S g (T x) pre (x > 0) { return S (x); }

static_assert (g (3).v == 3);

int
main ()
{
  volatile int x = 4;
  if (f (int (x)) != 5 || g (int (x)).v != 4)
    __builtin_abort ();
}
