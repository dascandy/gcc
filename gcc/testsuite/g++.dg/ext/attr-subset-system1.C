// { dg-do compile { target c++17 } }
// The subsets never apply to code in a system header, even when it is
// parsed or instantiated where they are active.  Code outside the system
// header is still checked.

[[subset::remove(core)]] void sys_removed ();	// { dg-message "declared here" }

[[subset::define(all, no_empty_statement, throw_non_std_exception,
		 pointer_arithmetic, binary_op_on_bool, variadic_functions,
		 operator_comma_overload, operator_unary_ampersand_overload,
		 operator_boolean_binary_overload, reinterpret_cast, const_cast,
		 dynamic_cast, raw_new_delete, core)]];
[[subset::activate(all)]];

#include "attr-subset-system1.h"

void
use (int *p, bool a, sys_comma c, sys_addr s)
{
  sys_all (p, a, a, p, c, s);
  sys_template (p, a);
  c, 1;						// { dg-error "using overloaded 'operator,'" }
  (void) &s;					// { dg-error "using overloaded unary 'operator&'" }
  p = p + 1;					// { dg-error "adding an integer to a pointer" }
  sys_removed ();				// { dg-error "was subset-removed" }
  int *n = new int;				// { dg-error "'new' expression" }
  delete n;					// { dg-error "'delete' expression" }
}

// The notes for the overloaded operators point into the system header.
// { dg-message "declared here" "" { target *-*-* } 0 }
