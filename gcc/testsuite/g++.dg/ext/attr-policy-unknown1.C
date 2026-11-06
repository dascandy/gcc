// { dg-do compile { target c++17 } }
// A policy group can be activated or suppressed before what makes it known,
// such as the policy::remove in a header included later.  Whether it is
// unknown is only decided at the end of the translation unit.

[[policy::activate(lib_checked)]];
[[policy::suppress(lib_other)]];

#include "attr-policy-unknown1.h"

void
f ()
{
  lib_unchecked ();		// { dg-error "was policy-removed" }
  lib_other_fn ();
}

// Still unknown at the end of the translation unit: a similar known name is
// suggested.
[[policy::activate(lib_chekced)]];	// { dg-warning "unknown policy 'lib_chekced'; did you mean 'lib_checked'\\?" }
// The warning is given once, at the first use.
[[policy::suppress(lib_chekced)]];

[[policy::activate(no_goot)]];		// { dg-warning "unknown policy 'no_goot'; did you mean 'no_goto'\\?" }

// No similar known name.
[[policy::activate(zzzzzzzzzzzz)]];	// { dg-warning "unknown policy 'zzzzzzzzzzzz'" }

// In a template, the warning is given for the definition, not for each
// instantiation.
template <typename T>
void
t ()
{
  [[policy::activate(tmpl_unknown)]];	// { dg-warning "unknown policy 'tmpl_unknown'" }
}
template void t<int> ();
template void t<long> ();
