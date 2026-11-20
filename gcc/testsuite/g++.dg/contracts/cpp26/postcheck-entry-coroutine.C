// -fcontract-postcheck-entry: a coroutine's preconditions are checked at the
// start of its ramp, so it is split like any other function.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcoroutines -fcontract-postcheck-entry -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <coroutine>

static int npre, npost, ran;
static bool P (bool b) { ++npre; return b; }
static bool Q (bool b) { ++npost; return b; }

struct task
{
  struct promise_type
  {
    task get_return_object () { return {}; }
    std::suspend_never initial_suspend () { return {}; }
    std::suspend_never final_suspend () noexcept { return {}; }
    void return_void () {}
    void unhandled_exception () {}
  };
};

task coro (int x) pre (P (x > 0)) post (Q (true)) { ran = x; co_return; }

int main ()
{
  coro (3);
  if (ran != 3 || npre != 1 || npost != 1)
    __builtin_abort ();
}
