// -fcontract-postcheck-entry: a violated precondition is still reported
// once, at the usual entry point, and under observe the body still runs.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

static int violations, body;

void
handle_contract_violation (const std::contracts::contract_violation &)
{
  ++violations;
}

int f (const int x) pre (x > 0) post (r: r == x) { ++body; return x; }

int main ()
{
  if (f (-1) != -1 || violations != 1 || body != 1)
    __builtin_abort ();
}
