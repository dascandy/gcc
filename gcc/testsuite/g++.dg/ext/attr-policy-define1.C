// { dg-do compile { target c++17 } }
// { dg-options "-Wredundant-policy" }
// policy::define names a set of policies that are activated or suppressed
// together.

[[policy::remove(core)]] void f ();
[[policy::remove(extra)]] void g ();

[[policy::define(safe, no_empty_statement, throw_non_std_exception)]];
// A definition can include other definitions.
[[policy::define(strict, safe, core)]];
[[using policy: define(all, strict, extra)]];

void
use (int x)
{
  [[policy::activate(strict)]];
  ;				// { dg-error "empty statement" }
  if (x)
    throw 1;			// { dg-error "throwing 'int'" }
  f ();				// { dg-error "'void f\\(\\)' was policy-removed" }
  g ();
  {
    [[policy::suppress(safe)]];
    ;
    throw 1;
    f ();			// { dg-error "'void f\\(\\)' was policy-removed" }
    [[policy::activate(no_empty_statement)]];
    ;				// { dg-error "empty statement" }
  }
  {
    [[policy::activate(all)]];
    g ();			// { dg-error "'void g\\(\\)' was policy-removed" }
  }
  [[policy::suppress(core)]];
  f ();
  ;				// { dg-error "empty statement" }
}

// A policy can be included both directly and through other definitions;
// it is still activated and suppressed as one policy.
[[policy::define(overlap, safe, no_empty_statement)]];
[[policy::define(overlap2, strict, safe, core)]];
void
use_overlap ()
{
  [[policy::activate(overlap2)]];
  ;				// { dg-error "empty statement" }
  f ();				// { dg-error "'void f\\(\\)' was policy-removed" }
  [[policy::suppress(overlap)]];
  ;
  f ();				// { dg-error "'void f\\(\\)' was policy-removed" }
  [[policy::suppress(core)]];
  [[policy::suppress(overlap2)]];	// { dg-warning "policy 'overlap2' is already suppressed" }
}

// Templates replay the expansion when instantiated.
template <typename T>
void
t ()
{
  [[policy::activate(strict)]];
  f ();				// { dg-error "'void f\\(\\)' was policy-removed" }
}
template void t<int> ();

// -Wredundant-policy warns only if nothing would change.
void
redundant ()
{
  [[policy::activate(no_empty_statement)]];
  [[policy::activate(safe)]];
  [[policy::activate(safe)]];		// { dg-warning "policy 'safe' is already active" }
  [[policy::suppress(throw_non_std_exception)]];
  [[policy::suppress(safe)]];
  [[policy::suppress(safe)]];		// { dg-warning "policy 'safe' is already suppressed" }
}

// Errors.
[[policy::define(safe, core)]];		// { dg-error "policy 'safe' is already known" }
[[policy::define(core, extra)]];	// { dg-error "policy 'core' is already known" }
[[policy::define(bad1, nonexistent)]];	// { dg-error "unknown policy 'nonexistent'" }
[[policy::define(bad2, bad2)]];		// { dg-error "unknown policy 'bad2'" }
[[policy::define(bad3, core, core)]];	// { dg-error "policy 'core' is listed more than once in the definition of 'bad3'" }
[[policy::define(bad4, core, extra, core)]];	// { dg-error "policy 'core' is listed more than once" }
[[policy::define(bad5, safe, strict, safe)]];	// { dg-error "policy 'safe' is listed more than once" }
[[policy::activate(bad3)]];		// { dg-warning "unknown policy 'bad3'" }
[[policy::define]];			// { dg-error "requires a policy group name and at least one policy group" }
[[policy::define(bad6)]];		// { dg-error "requires a policy group name and at least one policy group" }
[[policy::define()]];			// { dg-error "requires policy group names" }
[[policy::define(bad7, 1)]];		// { dg-error "requires policy group names" }
[[policy::define(bad8, core extra)]];	// { dg-error "requires policy group names" }
[[policy::define(bad9, core,)]];	// { dg-error "requires policy group names" }

void
block ()
{
  [[policy::define(local, core)]];	// { dg-error "must appear at namespace scope" }
}

namespace N
{
  [[policy::define(in_namespace, core)]];
}
[[policy::activate(in_namespace)]];
[[policy::suppress(in_namespace)]];

[[policy::define(misplaced, core)]] void h ();	// { dg-warning "must appear as a separate declaration at namespace scope" }
struct S
{
  [[policy::define(member, core)]] int i;	// { dg-warning "must appear as a separate declaration at namespace scope" }
};

// Something cannot be removed under a defined policy.
[[policy::remove(safe)]] void r1 ();		// { dg-error "cannot remove under policy group 'safe', which is defined by 'policy::define'" }
[[policy::remove(core), policy::remove(strict)]] void r2 ();	// { dg-error "cannot remove under policy group 'strict'" }
