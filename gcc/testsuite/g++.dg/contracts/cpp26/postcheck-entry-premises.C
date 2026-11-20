// -fcontract-postcheck-entry: wherever the postcheck entry point is entered
// from, the preconditions that the function enforces or assumes hold, so
// the body there may still rely on them.  One that is only observed may
// not hold.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -O2 -fdump-tree-optimized" }
// { dg-additional-options {-fcontract-configuration=\[{"match":{"namespace":"ob"},"output":{"semantic":"observe"}},{"match":{},"output":{"semantic":"enforce"}}\]} }

void dead_enforced ();
void live_observed ();

int f (int x) pre (x > 0)
{
  if (x <= 0)
    dead_enforced ();
  return x;
}

namespace ob {
int f (int x) pre (x > 0)
{
  if (x <= 0)
    live_observed ();
  return x;
}
}

// { dg-final { scan-tree-dump-not "dead_enforced" "optimized" } }
// { dg-final { scan-tree-dump "live_observed" "optimized" } }
