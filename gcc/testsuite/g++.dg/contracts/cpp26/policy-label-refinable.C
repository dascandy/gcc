// The refinable-body rule with a `policy (group)' term: whether the group is
// active depends on where the declaration is, so it can differ between
// translation units, and an active term must not settle the evaluation the
// way a label's allowed_semantics facet does (contract_evaluation_settled_p).
//
// policy-label-refinable-u.cc holds the callers and the definition they are
// compiled with, where grp_a is active: the label allows observe, enforce and
// quick_enforce, the active term takes observe out, and on a noexcept
// function {enforce, quick_enforce} is what a label would settle.
// policy-label-refinable-k.cc, linked first, holds the definition the linker
// keeps, where grp_a is not active, so it observes and continues: the
// callers must not use their definition's facts, and use_nx (-5) must be 0.
//
// -fno-ipa-sra: otherwise the callers call a local clone of their own
// translation unit's definition, which hides the problem.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p3400 -O2 -fno-ipa-sra -fcontract-evaluation-semantic=observe" }
// { dg-additional-sources "policy-label-refinable-k.cc policy-label-refinable-u.cc" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

int handled;

void
handle_contract_violation (const std::contracts::contract_violation &)
{
  ++handled;
}

int use_nx (int);

int
main ()
{
  if (use_nx (-5) != 0 || handled != 1)
    __builtin_abort ();
}
