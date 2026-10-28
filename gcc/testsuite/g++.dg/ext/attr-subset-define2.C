// { dg-do compile { target c++17 } }
// { dg-options "-Wredundant-subset" }
// A subset group can be activated or suppressed before subset::define
// defines it.  The change applies to the subsets it expands to from the
// definition on.

[[subset::activate(late)]];

void
before ()
{
  ;
  goto out;
 out:;
}

[[subset::define(late, no_empty_statement, no_goto)]];

void
after ()
{
  ;				// { dg-error "empty statement" }
  goto out;			// { dg-error "'goto' statement in subset 'no_goto'" }
 out:;
}

void
member_suppressed ()
{
  [[subset::suppress(no_goto)]];
  ;				// { dg-error "empty statement" }
  goto out;
 out:;
}

void
group_suppressed ()
{
  [[subset::suppress(late)]];
  ;
  goto out;
 out:;
}

// The earlier activation of the group makes activating it again redundant.
[[subset::activate(late)]];	// { dg-warning "subset 'late' is already active" }

[[subset::suppress(late)]];

// A removal under a subset group that a later definition includes.
[[subset::activate(late3)]];
[[subset::remove(late3_member)]] void late3_fn ();
[[subset::define(late3, late3_member)]];
void
use_late3 ()
{
  late3_fn ();			// { dg-error "was subset-removed" }
}

// A template defined before the definition and instantiated after it.
[[subset::suppress(late3)]];
[[subset::activate(late4)]];
[[subset::remove(late4_member)]] void late4_fn (int);
template <typename T>
void
t4 ()
{
  late4_fn (T ());		// { dg-error "was subset-removed" }
}
[[subset::define(late4, late4_member)]];
template void t4<int> ();
