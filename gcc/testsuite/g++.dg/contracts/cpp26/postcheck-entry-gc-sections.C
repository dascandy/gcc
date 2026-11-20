// -fcontract-postcheck-entry: when every call checks the preconditions at
// the call site, nothing refers to the usual entry point of the function,
// and the linker discards it with --gc-sections.  Only the postcheck entry
// point, which holds the body, is left in the program.
// { dg-do run { target { c++26 && { gc_sections && { i?86-*-linux* x86_64-*-linux* } } } } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -fcontracts-client-check=pre -fcontract-evaluation-semantic=enforce -O2 -ffunction-sections -Wl,--gc-sections" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

__attribute__((noinline)) int f (int x) pre (x > 0) { return x + 1; }
__attribute__((noinline)) inline int il (int x) pre (x > 0) { return x * 2; }

int
main (int argc, char **)
{
  return f (argc) + il (argc) == 4 ? 0 : 1;
}

// { dg-final { scan-symbol {(?n)^\S+ T _Z1fi\.postcheck$} } }
// { dg-final { scan-symbol-not {(?n)^\S+ T _Z1fi$} } }
// { dg-final { scan-symbol {(?n)^\S+ W _Z2ili\.postcheck$} } }
// { dg-final { scan-symbol-not {(?n)^\S+ [TW] _Z2ili$} } }
