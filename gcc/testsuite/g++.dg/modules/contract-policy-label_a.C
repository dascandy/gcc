// `policy (group)' terms on contracts in a module interface: whether the
// group was active where each assertion is written travels with the
// assertion through the CMI.  contract-policy-label_b.C imports the module,
// with no policy active, and checks the semantics.

// { dg-do compile { target c++26 } }
// { dg-additional-options "-fmodules -fcontract-evaluation-semantic=observe" }

export module contract_policy_label;
// { dg-module-cmi "contract_policy_label" }

[[policy::remove(grp_a)]] void unused_a ();

export inline int off (int x) pre <policy(grp_a)> (x > 0) { return x; }

[[policy::activate(grp_a)]];

export inline int on (int x) pre <policy(grp_a)> (x > 0) { return x; }
export template<typename T>
T tmpl_on (T x) pre <policy(grp_a)> (x > 0) { return x; }
export template<typename T>
T body_on (T x) { contract_assert <policy(grp_a)> (x > 0); return x; }
