// What a `policy (group)' term is, with -fcontracts-p3400: only a whole
// operand of the label's top-level `|'.  A function named `policy' can be
// called in a label as `::policy (x)', `ns::policy (x)' or `(policy (x))',
// and `policy (x)' inside a template-argument-list or other brackets is an
// ordinary expression.  `>>' can close both a template-argument-list and the
// specifier, before or after a term.
// { dg-do compile { target c++26 } }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
// { dg-additional-options "-fcontracts-p3400" }

#include <contracts>

[[policy::remove(grp_a)]] void unused_a ();

template<typename T> struct plain_t { using assertion_control_object = plain_t; };
template<typename T> constexpr plain_t<T> plain{};

template<int N> struct num_t { using assertion_control_object = num_t; };
template<int N> constexpr num_t<N> num{};

constexpr plain_t<int> policy (int) { return {}; }
namespace ns { constexpr plain_t<int> policy (int) { return {}; } }
constexpr int v = 0;
constexpr int count (plain_t<int>) { return 1; }

void a (int x) pre <::policy (v)> (x > 0);
void b (int x) pre <ns::policy (v)> (x > 0);
void c (int x) pre <(policy (v))> (x > 0);
void d (int x) pre <plain<int> | ::policy (v)> (x > 0);
void e (int x) pre <num<count (policy (v))>> (x > 0);
void f (int x) pre <plain<int> | policy(grp_a)> (x > 0);
void g (int x) pre <policy(grp_a) | plain<int>> (x > 0);
void h (int x) pre <num<(1 > 0)> | policy(grp_a)> (x > 0);

struct S
{
  void m (int x) pre <::policy (v) | policy(grp_a)> (x > 0);
  void n (int x) pre <policy(grp_a) | plain<int>> (x > 0);
};
