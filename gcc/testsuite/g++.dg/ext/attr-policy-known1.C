// { dg-do compile { target c++17 } }
// Only known policies can be activated or suppressed: the predefined ones,
// and those that something was removed under.

// Predefined.
[[policy::activate(no_empty_statement)]];
[[policy::suppress(no_empty_statement)]];
[[policy::activate(throw_non_std_exception)]];
[[policy::suppress(throw_non_std_exception)]];
[[policy::activate(pointer_arithmetic)]];
[[policy::suppress(pointer_arithmetic)]];
[[policy::activate(binary_op_on_bool)]];
[[policy::suppress(binary_op_on_bool)]];
[[policy::activate(no_boolean_ordering)]];
[[policy::suppress(no_boolean_ordering)]];
[[policy::activate(raw_new_delete)]];
[[policy::suppress(raw_new_delete)]];
[[policy::activate(no_goto)]];
[[policy::suppress(no_goto)]];
[[policy::activate(no_wchar_t)]];
[[policy::suppress(no_wchar_t)]];
[[policy::activate(no_mixed_switch)]];
[[policy::suppress(no_mixed_switch)]];
[[policy::activate(variadic_functions)]];
[[policy::suppress(variadic_functions)]];
[[policy::activate(operator_comma_overload)]];
[[policy::suppress(operator_comma_overload)]];
[[policy::activate(operator_unary_ampersand_overload)]];
[[policy::suppress(operator_unary_ampersand_overload)]];
[[policy::activate(operator_boolean_binary_overload)]];
[[policy::suppress(operator_boolean_binary_overload)]];
[[policy::activate(reinterpret_cast)]];
[[policy::suppress(reinterpret_cast)]];
[[policy::activate(const_cast)]];
[[policy::suppress(const_cast)]];
[[policy::activate(dynamic_cast)]];
[[policy::suppress(dynamic_cast)]];

// Not known yet.
[[policy::activate(core)]];			// { dg-error "unknown policy 'core'" }
[[policy::suppress(core)]];			// { dg-error "unknown policy 'core'" }

[[policy::remove(core)]] void f ();
[[policy::activate(core)]];
[[policy::suppress(core)]];

// A removal from another kind of declaration, a class or a variable.
struct [[policy::remove(cls)]] C {};
[[policy::remove(var)]] int v;
[[policy::activate(cls), policy::activate(var)]];

// An ignored removal does not make a policy known.
[[policy::remove(ignored)]] extern int e;	// { dg-warning "only applies to" }
[[policy::activate(ignored)]];			// { dg-error "unknown policy 'ignored'" }

void
g ()
{
  [[policy::activate(never)]];			// { dg-error "unknown policy 'never'" }
  {
    [[policy::suppress(never)]];		// { dg-error "unknown policy 'never'" }
  }
  [[policy::activate(core)]];
}

template <typename T>
void
t ()
{
  [[policy::activate(never)]];			// { dg-error "unknown policy 'never'" }
}
template void t<int> ();
