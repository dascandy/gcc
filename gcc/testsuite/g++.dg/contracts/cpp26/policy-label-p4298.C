// Under -fcontracts-p4298 an active `policy (group)' term also allows
// noexcept_enforce: a configured noexcept_observe becomes noexcept_enforce,
// which ends the program after the handler returns.  While the group is not
// active, noexcept_observe is used.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p4298 -fcontract-evaluation-semantic=noexcept_observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
#include <cstdlib>

using std::contracts::evaluation_semantic;

[[policy::remove(grp_a)]] void unused_a ();

static int handled;

void handle_contract_violation (const std::contracts::contract_violation &v)
{
  ++handled;
  if (handled == 1 && v.semantic () == evaluation_semantic::noexcept_observe)
    return;
  if (handled == 2 && v.semantic () == evaluation_semantic::noexcept_enforce)
    std::_Exit (0);
  std::abort ();
}

int off (int x) pre <policy(grp_a)> (x > 0) { return x; }
[[policy::activate(grp_a)]];
int on (int x) pre <policy(grp_a)> (x > 0) { return x; }

int
main ()
{
  off (-1);
  on (-1);
  std::abort ();
}
