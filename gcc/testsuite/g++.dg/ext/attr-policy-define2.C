// { dg-do compile { target c++17 } }
// { dg-options "-Wredundant-policy" }
// A policy group can be activated or suppressed before policy::define
// defines it.  The change applies to the policies it expands to from the
// definition on.

[[policy::activate(late)]];

void
before ()
{
  ;
  goto out;
 out:;
}

[[policy::define(late, no_empty_statement, no_goto)]];

void
after ()
{
  ;				// { dg-error "empty statement" }
  goto out;			// { dg-error "'goto' statement in policy 'no_goto'" }
 out:;
}

void
member_suppressed ()
{
  [[policy::suppress(no_goto)]];
  ;				// { dg-error "empty statement" }
  goto out;
 out:;
}

void
group_suppressed ()
{
  [[policy::suppress(late)]];
  ;
  goto out;
 out:;
}

// The earlier activation of the group makes activating it again redundant.
[[policy::activate(late)]];	// { dg-warning "policy 'late' is already active" }

[[policy::suppress(late)]];

// A removal under a policy group that a later definition includes.
[[policy::activate(late3)]];
[[policy::remove(late3_member)]] void late3_fn ();
[[policy::define(late3, late3_member)]];
void
use_late3 ()
{
  late3_fn ();			// { dg-error "was policy-removed" }
}

// A template defined before the definition and instantiated after it.
[[policy::suppress(late3)]];
[[policy::activate(late4)]];
[[policy::remove(late4_member)]] void late4_fn (int);
template <typename T>
void
t4 ()
{
  late4_fn (T ());		// { dg-error "was policy-removed" }
}
[[policy::define(late4, late4_member)]];
template void t4<int> ();
