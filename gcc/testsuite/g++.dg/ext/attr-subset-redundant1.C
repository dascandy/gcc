// { dg-do compile { target c++17 } }
// { dg-options "-Wredundant-subset" }
// -Wredundant-subset only counts the changes made in the same file, so that
// whether a change warns does not depend on what the translation unit
// includes, or in what order.

[[subset::activate(no_wchar_t)]];
[[subset::activate(pointer_arithmetic)]];

#include "attr-subset-redundant1.h"

// The last change made in this file already activated it.
[[subset::activate(no_wchar_t)]];	// { dg-warning "subset 'no_wchar_t' is already active" }

// Only the header activated it.
[[subset::activate(raw_new_delete)]];
[[subset::activate(raw_new_delete)]];	// { dg-warning "subset 'raw_new_delete' is already active" }

// Only the header suppressed it.
[[subset::suppress(no_goto)]];
[[subset::suppress(no_goto)]];		// { dg-warning "subset 'no_goto' is already suppressed" }

// The header suppressed it after this file activated it, so activating it
// again changes the active set.
[[subset::activate(pointer_arithmetic)]];

// Never activated: suppressing it is not redundant.
[[subset::suppress(no_mixed_switch)]];
[[subset::suppress(no_mixed_switch)]];	// { dg-warning "subset 'no_mixed_switch' is already suppressed" }

void
f ()
{
  // The change at namespace scope counts.
  [[subset::activate(pointer_arithmetic)]];	// { dg-warning "subset 'pointer_arithmetic' is already active" }
  [[subset::activate(no_empty_statement)]];
  {
    [[subset::activate(no_empty_statement)]];	// { dg-warning "subset 'no_empty_statement' is already active" }
  }
}
