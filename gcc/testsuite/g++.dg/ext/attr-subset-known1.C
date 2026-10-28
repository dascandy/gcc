// { dg-do compile { target c++17 } }
// The known subsets are the predefined ones, and those that something was
// removed under.  A subset that is still not known at the end of the
// translation unit gets a warning, once, at its first use.

// Predefined.
[[subset::activate(no_empty_statement)]];
[[subset::suppress(no_empty_statement)]];
[[subset::activate(throw_non_std_exception)]];
[[subset::suppress(throw_non_std_exception)]];
[[subset::activate(pointer_arithmetic)]];
[[subset::suppress(pointer_arithmetic)]];
[[subset::activate(binary_op_on_bool)]];
[[subset::suppress(binary_op_on_bool)]];
[[subset::activate(no_boolean_ordering)]];
[[subset::suppress(no_boolean_ordering)]];
[[subset::activate(raw_new_delete)]];
[[subset::suppress(raw_new_delete)]];
[[subset::activate(no_goto)]];
[[subset::suppress(no_goto)]];
[[subset::activate(no_wchar_t)]];
[[subset::suppress(no_wchar_t)]];
[[subset::activate(no_mixed_switch)]];
[[subset::suppress(no_mixed_switch)]];
[[subset::activate(variadic_functions)]];
[[subset::suppress(variadic_functions)]];
[[subset::activate(operator_comma_overload)]];
[[subset::suppress(operator_comma_overload)]];
[[subset::activate(operator_unary_ampersand_overload)]];
[[subset::suppress(operator_unary_ampersand_overload)]];
[[subset::activate(operator_boolean_binary_overload)]];
[[subset::suppress(operator_boolean_binary_overload)]];
[[subset::activate(reinterpret_cast)]];
[[subset::suppress(reinterpret_cast)]];
[[subset::activate(const_cast)]];
[[subset::suppress(const_cast)]];
[[subset::activate(dynamic_cast)]];
[[subset::suppress(dynamic_cast)]];

// Not known yet, but known by the end of the translation unit.
[[subset::activate(core)]];
[[subset::suppress(core)]];

[[subset::remove(core)]] void f ();
[[subset::activate(core)]];
[[subset::suppress(core)]];

// A removal from another kind of declaration, a class or a variable.
struct [[subset::remove(cls)]] C {};
[[subset::remove(var)]] int v;
[[subset::activate(cls), subset::activate(var)]];

// An ignored removal does not make a subset known.
[[subset::remove(ignored)]] extern int e;	// { dg-warning "only applies to" }
[[subset::activate(ignored)]];			// { dg-warning "unknown subset 'ignored'" }

void
g ()
{
  [[subset::activate(never)]];			// { dg-warning "unknown subset 'never'" }
  {
    [[subset::suppress(never)]];
  }
  [[subset::activate(core)]];
}

template <typename T>
void
t ()
{
  [[subset::activate(never)]];
}
template void t<int> ();
