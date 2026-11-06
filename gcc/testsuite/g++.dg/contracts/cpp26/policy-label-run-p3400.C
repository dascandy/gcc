// `policy (group)' terms combined with a P3400 label: a term is an operand of
// the label's top-level `|', in any position, and while its group is active
// the semantics the label allows are intersected with the terminating ones.
// Also `>>' closing both a template-argument-list in the label and the
// specifier, and a label deferred with the predicate of a member function.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p3400 -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
#include <cstdlib>

using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;
using namespace std::contracts::labels;

[[policy::remove(grp_a)]] void unused_a ();
[[policy::remove(grp_b)]] void unused_b ();

// A label with no facets.
template<typename T> struct plain_t { using assertion_control_object = plain_t; };
template<typename T> constexpr plain_t<T> plain{};

// A label allowing observe and enforce.
struct open_t {
  using assertion_control_object = open_t;
  static constexpr evaluation_semantic_set allowed_semantics
    = {evaluation_semantic::observe, evaluation_semantic::enforce};
};
constexpr open_t open_lbl{};

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

int lead_off (int x) pre <policy(grp_a) | plain<int>> (x > 0) { return x; }
int trail_off (int x) pre <open_lbl | policy(grp_a)> (x > 0) { return x; }

[[policy::activate(grp_a)]];

int lead_on (int x) pre <policy(grp_a) | plain<int>> (x > 0) { return x; }
int trail_on (int x) pre <plain<int> | policy(grp_a)> (x > 0) { return x; }
int mid_on (int x) pre <open_lbl | policy(grp_a) | plain<int>> (x > 0)
{ return x; }
int many_on (int x) pre <policy(grp_b) | open_lbl | policy(grp_a)> (x > 0)
{ return x; }
int term_on (int x) pre <terminating | policy(grp_a)> (x > 0) { return x; }
void assert_on (int x) { contract_assert <open_lbl | policy(grp_a)> (x > 0); }

struct S
{
  int mem_on (int x) pre <plain<int> | policy(grp_a)> (x > 0) { return x; }
  [[policy::suppress(grp_a)]];
  int mem_off (int x) pre <policy(grp_a) | open_lbl> (x > 0) { return x; }
};

template<typename T>
T tmpl_on (T x) pre <plain<T> | policy(grp_a)> (x > 0) { return x; }

int
main ()
{
  CHECK (lead_off (-1), observe);
  CHECK (trail_off (-1), observe);
  CHECK (lead_on (-1), enforce);
  CHECK (trail_on (-1), enforce);
  CHECK (mid_on (-1), enforce);
  CHECK (many_on (-1), enforce);
  CHECK (term_on (-1), enforce);
  CHECK ((assert_on (-1), 0), enforce);
  CHECK (S ().mem_on (-1), enforce);
  CHECK (S ().mem_off (-1), observe);
  CHECK (tmpl_on (-1), enforce);
}
