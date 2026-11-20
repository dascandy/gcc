// A caller-side check of the precondition of a lambda reads the lambda's
// captures through the closure parameter of the wrapper, not through that
// of the lambda's operator() (ICE in expand_expr_real_1 for a capture of
// this).  The violation is reported by the caller and by the lambda.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-client-check=pre -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

int violations;

void
handle_contract_violation (const std::contracts::contract_violation &)
{
  ++violations;
}

struct S
{
  int m = 5;
  bool f () { return [this] (int x) pre (x > m) { return x > 0; } (1); }
};

template <class T>
struct U
{
  T m = 5;
  bool r = [this] (int x) pre (x > m) { return x > 0; } (1);
};

int
main ()
{
  S s;
  if (!s.f () || violations != 2)
    __builtin_abort ();
  U<int> u;
  if (!u.r || violations != 4)
    __builtin_abort ();
}
