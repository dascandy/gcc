// Errors for `policy (group)' terms with -fcontracts-p3400: a term that is
// not an operand of the top-level `|', an operator binding less tightly than
// `|' next to the terms, a term without a group name, a label whose allowed
// semantics none of the terminating ones meet while the policy is active, and
// a compute_semantic result outside the semantics the active policy allows.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;
using namespace std::contracts::labels;

[[policy::remove(grp_a)]] void unused_a ();

struct observe_t {
  using assertion_control_object = observe_t;
  static constexpr evaluation_semantic_set allowed_semantics
    = {evaluation_semantic::observe};
};
constexpr observe_t observe_lbl{};

struct to_observe_t {
  using assertion_control_object = to_observe_t;
  constexpr evaluation_semantic
  compute_semantic (evaluation_semantic) const
  { return evaluation_semantic::observe; }
};
constexpr to_observe_t to_observe{};

// Inactive: the label's set stands.
void off1 (int x) pre <observe_lbl | policy(grp_a)> (x > 0);
void off2 (int x) pre <to_observe | policy(grp_a)> (x > 0) { }

[[policy::activate(grp_a)]];

void e1 (int x) pre <observe_lbl | policy(grp_a)> (x > 0); // { dg-error "allows none of the evaluation semantics that active policy .grp_a. permits" }
void e2 (int x) pre <to_observe | policy(grp_a)> (x > 0) { } // { dg-error "compute_semantic. result is not in the allowed" }
// { dg-message "restricted by active policy .grp_a." "" { target *-*-* } .-1 }

void e3 (int x) pre <observe_lbl ^ policy(grp_a)> (x > 0); // { dg-error "must be an operand of the top-level" }
void e4 (int x) pre <policy(grp_a) + 1> (x > 0); // { dg-error "must be an operand of the top-level" }
void e5 (int x) pre <true ? observe_lbl : observe_lbl | policy(grp_a)> (x > 0); // { dg-error "binds less tightly than" }
// { dg-message "parenthesize" "" { target *-*-* } .-1 }
// The comma is inside the operand of the top-level `|'.
void e6 (int x) pre <policy(grp_a), observe_lbl> (x > 0); // { dg-error "must be an operand of the top-level" }
void e6b (int x) pre <observe_lbl, observe_lbl | policy(grp_a)> (x > 0); // { dg-error "binds less tightly than" }
// { dg-message "parenthesize" "" { target *-*-* } .-1 }
void e7 (int x) pre <policy(1)> (x > 0); // { dg-error "expected a policy group name" }
void e8 (int x) pre <observe_lbl | policy(grp_a::b)> (x > 0); // { dg-error "expected a policy group name" }
void e9 (int x) { contract_assert <observe_lbl | policy(grp_a)> (x > 0); } // { dg-error "allows none of the evaluation semantics" }

template<typename T>
struct observe_tmpl_t {
  using assertion_control_object = observe_tmpl_t;
  static constexpr evaluation_semantic_set allowed_semantics
    = {evaluation_semantic::observe};
};
template<typename T> constexpr observe_tmpl_t<T> observe_tmpl{};

// A dependent label is checked when it is substituted.
template<typename T>
void t (T x) pre <observe_tmpl<T> | policy(grp_a)> (x > 0) { } // { dg-error "allows none of the evaluation semantics" }
void use () { t (1); }
