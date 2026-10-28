// Included by attr-subset-redundant1.C.  None of these changes repeats an
// earlier change made in this file, so none of them is redundant, whatever
// the including file did.

// The including file activated no_wchar_t; activating it again is not
// redundant here.
[[subset::activate(no_wchar_t)]];

// Nothing activated no_goto; suppressing it is not redundant either.
[[subset::suppress(no_goto)]];

[[subset::activate(raw_new_delete)]];

// The including file activated pointer_arithmetic.
[[subset::suppress(pointer_arithmetic)]];
