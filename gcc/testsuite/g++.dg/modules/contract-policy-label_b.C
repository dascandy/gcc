// Consumer for contract-policy-label_a.C: the assertions written where
// grp_a was active are enforced, the other observes, and the templates are
// instantiated here, where it is not active.

// { dg-module-do run { target c++26 } }
// { dg-additional-options "-fmodules -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

import contract_policy_label;
#include <contracts>
#include <cstdlib>

using std::contracts::evaluation_semantic;

static evaluation_semantic last;

void handle_contract_violation (const std::contracts::contract_violation &v)
{
  last = v.semantic ();
  throw 0;
}

#define CHECK(E, S) \
  do								\
    {								\
      try { (void) (E); std::abort (); } catch (int) { }	\
      if (last != evaluation_semantic::S)			\
	std::abort ();						\
    }								\
  while (0)

int main ()
{
  CHECK (off (-1), observe);
  CHECK (on (-1), enforce);
  CHECK (tmpl_on (-1), enforce);
  CHECK (body_on (-1), enforce);
}
