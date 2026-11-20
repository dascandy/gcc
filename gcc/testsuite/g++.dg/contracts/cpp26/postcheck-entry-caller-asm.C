// -fcontract-postcheck-entry: a caller-side wrapper that enforces every
// precondition calls the postcheck entry point; one that observes one
// calls the function.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -fcontract-postcheck-entry -O0 -fno-asynchronous-unwind-tables" }
// { dg-additional-options {-fcontract-configuration=\[{"match":{"caller":true,"namespace":"en"},"output":{"semantic":"enforce"}},{"match":{"caller":true,"namespace":"ob"},"output":{"semantic":"observe"}}\]} }

namespace en { int f (int x) pre (x > 0) { return x; } }
namespace ob { int f (int x) pre (x > 0) { return x; } }

int use (int x) { return en::f (x) + ob::f (x); }

// { dg-final { scan-assembler {\n_ZN2en1fEi\.contract_wrapper:\n(?:(?!\.size)[^\n]*\n)*?\t(?:call|jmp)\t_ZN2en1fEi\.postcheck\n} } }
// { dg-final { scan-assembler {\n_ZN2ob1fEi\.contract_wrapper:\n(?:(?!\.size)[^\n]*\n)*?\t(?:call|jmp)\t_ZN2ob1fEi\n} } }
