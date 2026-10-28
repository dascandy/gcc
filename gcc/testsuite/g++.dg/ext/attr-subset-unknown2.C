// { dg-do compile { target c++17 } }
// { dg-options "-Wno-unknown-subset" }
// -Wno-unknown-subset disables the warning about a subset group that is
// still unknown at the end of the translation unit.

[[subset::activate(never_known)]];
[[subset::suppress(never_known)]];
