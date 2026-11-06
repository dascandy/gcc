// { dg-do compile { target c++17 } }
// { dg-options "-Wno-unknown-policy" }
// -Wno-unknown-policy disables the warning about a policy group that is
// still unknown at the end of the translation unit.

[[policy::activate(never_known)]];
[[policy::suppress(never_known)]];
