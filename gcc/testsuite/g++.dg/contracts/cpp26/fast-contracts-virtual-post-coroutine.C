// -ffast-contracts with P3097: a coroutine that overrides B::f checks
// B::f's postconditions, which a virtual call through B::f assumes, on
// what its ramp returns, as any other function that overrides B::f does.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
#include <coroutine>
#include <cstdlib>
#include <cstring>

void
handle_contract_violation (const std::contracts::contract_violation &v)
{
  if (std::strcmp (v.comment (), "r.v > 10") == 0)
    std::exit (0);
  std::abort ();
}

struct Task
{
  int v;
  struct promise_type
  {
    Task get_return_object () { return Task { 7 }; }
    std::suspend_never initial_suspend () noexcept { return {}; }
    std::suspend_never final_suspend () noexcept { return {}; }
    void return_void () {}
    void unhandled_exception () {}
  };
};

struct B { virtual Task f (int x) pre (x > 0) post (r: r.v > 10); };
struct D : B { Task f (int x) override; };
Task B::f (int) { return Task { 11 }; }
Task D::f (int) { co_return; }

__attribute__ ((noipa)) int
call (B *b, int x)
{
  return b->f (x).v;
}

int
main ()
{
  D d;
  call (&d, 1);
  return 1;
}
