// A `policy (group)' term in an assertion-control-specifier: while the
// group is active where the assertion is written, the assertion may only use
// a terminating semantic, so a configured observe becomes enforce; while it
// is not, the term has no effect.  Each kind of assertion, several terms, a
// group defined by policy::define, block scope, member functions whose
// contracts are parsed at the end of the class, templates (including a
// policy change inside the template, replayed at instantiation), lambdas, a
// generic lambda and a system header.  No other paper flag: the policy label
// needs none.  The handler checks the semantic and throws.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
#include <cstdlib>

using std::contracts::evaluation_semantic;

// Policy groups for the test: naming them in policy::remove makes them known
// without disallowing anything the test uses.
[[policy::remove(grp_a)]] void unused_a ();
[[policy::remove(grp_b)]] void unused_b ();
[[policy::define(grp_ab, grp_a, grp_b)]];

static evaluation_semantic last;

void handle_contract_violation (const std::contracts::contract_violation &v)
{
  last = v.semantic ();
  throw 0;
}

#define CHECK(E, S) \
  do								\
    {								\
      try { (void) (E); std::abort (); } catch (int) { }	\
      if (last != evaluation_semantic::S)			\
	std::abort ();						\
    }								\
  while (0)

// Inactive.
int pre_off (int x) pre <policy(grp_a)> (x > 0) { return x; }

[[policy::activate(grp_a)]];

int pre_on (int x) pre <policy(grp_a)> (x > 0) { return x; }
int post_on (const int x) post <policy(grp_a)> (x > 0) { return x; }
int post_r_on (int x) post <policy(grp_a)> (r : r > 0) { return x; }
void assert_on (int x) { contract_assert <policy(grp_a)> (x > 0); }

// Several terms: restricted when any of them is active.
int two_on (int x) pre <policy(grp_b) | policy(grp_a)> (x > 0) { return x; }

// A defined group is active when every group it expands to is.
int ab_half (int x) pre <policy(grp_ab)> (x > 0) { return x; }

#include "policy-label-run.h"

// Activeness is decided where the assertion is, in a block too.
void block (int x, bool on)
{
  if (on)
    {
      [[policy::activate(grp_b)]];
      contract_assert <policy(grp_b)> (x > 0);
    }
  else
    contract_assert <policy(grp_b)> (x > 0);
}

struct S
{
  // Parsed at the end of the class, where grp_b is suppressed again, but
  // decided where it is written.
  [[policy::activate(grp_b)]];
  int mem_on (int x) pre <policy(grp_b)> (x > 0) { return x; }
  [[policy::suppress(grp_b)]];
  int mem_off (int x) pre <policy(grp_b)> (x > 0) { return x; }
};

template<typename T>
T tmpl_on (T x) pre <policy(grp_a)> (x > 0) { return x; }

auto lam_on = [] (int x) pre <policy(grp_a)> (x > 0) { return x; };
auto glam_on = [] (auto x) pre <policy(grp_a)> (x > 0) { return x; };

[[policy::activate(grp_b)]];
int ab_full (int x) pre <policy(grp_ab)> (x > 0) { return x; }
[[policy::suppress(grp_ab)]];

int two_off (int x) pre <policy(grp_b) | policy(grp_a)> (x > 0) { return x; }

template<typename T>
T tmpl_off (T x) pre <policy(grp_a)> (x > 0) { return x; }

template<typename T>
T tmpl_body (T x, bool on)
{
  if (!on)
    contract_assert <policy(grp_a)> (x > 0);
  [[policy::activate(grp_a)]];
  if (on)
    contract_assert <policy(grp_a)> (x > 0);
  return x;
}

auto lam_off = [] (int x) pre <policy(grp_a)> (x > 0) { return x; };

int
main ()
{
  CHECK (pre_off (-1), observe);
  CHECK (pre_on (-1), enforce);
  CHECK (post_on (-1), enforce);
  CHECK (post_r_on (-1), enforce);
  CHECK ((assert_on (-1), 0), enforce);
  CHECK (two_on (-1), enforce);
  CHECK (ab_half (-1), observe);
  CHECK (sys_f (-1), enforce);
  CHECK ((block (-1, true), 0), enforce);
  CHECK ((block (-1, false), 0), observe);
  CHECK (S ().mem_on (-1), enforce);
  CHECK (S ().mem_off (-1), observe);
  CHECK (tmpl_on (-1), enforce);
  CHECK (lam_on (-1), enforce);
  CHECK (glam_on (-1), enforce);
  CHECK (ab_full (-1), enforce);
  CHECK (two_off (-1), observe);
  CHECK (tmpl_off (-1), observe);
  CHECK (tmpl_body (-1, true), enforce);
  CHECK (tmpl_body (-1, false), observe);
  CHECK (lam_off (-1), observe);
}
