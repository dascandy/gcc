// Extra translation unit of policy-label-refinable.C, linked first: the
// definition the linker keeps, where grp_a is not active.

#include <contracts>

using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;

struct open_t {
  using assertion_control_object = open_t;
  static constexpr evaluation_semantic_set allowed_semantics
    = {evaluation_semantic::observe, evaluation_semantic::enforce,
       evaluation_semantic::quick_enforce};
};
inline constexpr open_t l_open{};

[[policy::remove(grp_a)]] void unused_a ();

[[gnu::noinline]] inline int
nx (int x) noexcept
{
  contract_assert <l_open | policy(grp_a)> (x > 0);	// observe
  return x;
}

int (*volatile keep[]) (int) = { nx };
