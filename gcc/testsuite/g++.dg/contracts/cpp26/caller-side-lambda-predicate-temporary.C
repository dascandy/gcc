// A caller-side check of a predicate that creates a temporary -- here the
// closure object of a lambda -- has a temporary of its own: the callee,
// which checks the same predicate, has the original in its body.  Both
// used to be gimplified as the same variable (ICE in gimple_add_tmp_var).
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-client-check=pre" }

int h1 (int x) pre ([x] () mutable { return ++x; } () > 0) { return x; }
int h2 (int x) pre ([x] () { return x; } () > 0) { return x; }

template <class T>
T t1 (T x) pre ([x] () mutable { return ++x; } () > 0) { return x; }

struct S
{
  int n;
  bool ok () const { return n > 0; }
};
template <class T>
int f_lambda (S s) pre ([&] { return s.ok (); } ()) { return s.n; }

int
main ()
{
  if (h1 (1) + h2 (2) + t1 (3) + f_lambda<int> (S{4}) != 10)
    __builtin_abort ();
}
