// { dg-do compile { target c++17 } }
// { dg-options "-Wredundant-policy" }
// -Wredundant-policy only counts the changes made in the same file, so that
// whether a change warns does not depend on what the translation unit
// includes, or in what order.

[[policy::activate(no_wchar_t)]];
[[policy::activate(pointer_arithmetic)]];

#include "attr-policy-redundant1.h"

// The last change made in this file already activated it.
[[policy::activate(no_wchar_t)]];	// { dg-warning "policy 'no_wchar_t' is already active" }

// Only the header activated it.
[[policy::activate(raw_new_delete)]];
[[policy::activate(raw_new_delete)]];	// { dg-warning "policy 'raw_new_delete' is already active" }

// Only the header suppressed it.
[[policy::suppress(no_goto)]];
[[policy::suppress(no_goto)]];		// { dg-warning "policy 'no_goto' is already suppressed" }

// The header suppressed it after this file activated it, so activating it
// again changes the active set.
[[policy::activate(pointer_arithmetic)]];

// Never activated: suppressing it is not redundant.
[[policy::suppress(no_mixed_switch)]];
[[policy::suppress(no_mixed_switch)]];	// { dg-warning "policy 'no_mixed_switch' is already suppressed" }

void
f ()
{
  // The change at namespace scope counts.
  [[policy::activate(pointer_arithmetic)]];	// { dg-warning "policy 'pointer_arithmetic' is already active" }
  [[policy::activate(no_empty_statement)]];
  {
    [[policy::activate(no_empty_statement)]];	// { dg-warning "policy 'no_empty_statement' is already active" }
  }
}
