// { dg-do compile { target c++17 } }
// A subset group can be activated or suppressed before what makes it known,
// such as the subset::remove in a header included later.  Whether it is
// unknown is only decided at the end of the translation unit.

[[subset::activate(lib_checked)]];
[[subset::suppress(lib_other)]];

#include "attr-subset-unknown1.h"

void
f ()
{
  lib_unchecked ();		// { dg-error "was subset-removed" }
  lib_other_fn ();
}

// Still unknown at the end of the translation unit: a similar known name is
// suggested.
[[subset::activate(lib_chekced)]];	// { dg-warning "unknown subset 'lib_chekced'; did you mean 'lib_checked'\\?" }
// The warning is given once, at the first use.
[[subset::suppress(lib_chekced)]];

[[subset::activate(no_goot)]];		// { dg-warning "unknown subset 'no_goot'; did you mean 'no_goto'\\?" }

// No similar known name.
[[subset::activate(zzzzzzzzzzzz)]];	// { dg-warning "unknown subset 'zzzzzzzzzzzz'" }

// In a template, the warning is given for the definition, not for each
// instantiation.
template <typename T>
void
t ()
{
  [[subset::activate(tmpl_unknown)]];	// { dg-warning "unknown subset 'tmpl_unknown'" }
}
template void t<int> ();
template void t<long> ();
