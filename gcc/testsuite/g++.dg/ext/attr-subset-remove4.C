// { dg-do compile { target c++20 } }
// A subset::remove attribute after the declarator appertains to the function
// type, not the function, so it is warned about and has no effect.

[[subset::remove(core)]] void removed ();

void f () [[subset::remove(core)]] {}		// { dg-warning "only applies to" }
void g () [[subset::remove(core)]];		// { dg-warning "only applies to" }
void h () noexcept [[subset::remove(core)]] {}	// { dg-warning "only applies to" }
template <typename T>
void tf () [[subset::remove(core)]] {}		// { dg-warning "only applies to" }
struct S
{
  void m () [[subset::remove(core)]] {}		// { dg-warning "only applies to" }
  void m2 () [[subset::remove(core)]];		// { dg-warning "only applies to" }
};
auto l = [] () [[subset::remove(core)]] {};	// { dg-warning "only applies to" }
using F = void () [[subset::remove(core)]];	// { dg-warning "only applies to" }

void
use ()
{
  [[subset::activate(core)]];
  f ();
  g ();
  h ();
  tf<int> ();
  S ().m ();
  S ().m2 ();
  l ();
}
