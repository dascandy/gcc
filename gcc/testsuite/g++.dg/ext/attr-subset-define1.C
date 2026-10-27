// { dg-do compile { target c++17 } }
// { dg-options "-Wredundant-subset" }
// subset::define names a set of subsets that are activated or suppressed
// together.

[[subset::remove(core)]] void f ();
[[subset::remove(extra)]] void g ();

[[subset::define(safe, no_empty_statement, throw_non_std_exception)]];
// A definition can include other definitions.
[[subset::define(strict, safe, core)]];
[[using subset: define(all, strict, extra)]];

void
use (int x)
{
  [[subset::activate(strict)]];
  ;				// { dg-error "empty statement" }
  if (x)
    throw 1;			// { dg-error "throwing 'int'" }
  f ();				// { dg-error "'void f\\(\\)' was subset-removed" }
  g ();
  {
    [[subset::suppress(safe)]];
    ;
    throw 1;
    f ();			// { dg-error "'void f\\(\\)' was subset-removed" }
    [[subset::activate(no_empty_statement)]];
    ;				// { dg-error "empty statement" }
  }
  {
    [[subset::activate(all)]];
    g ();			// { dg-error "'void g\\(\\)' was subset-removed" }
  }
  [[subset::suppress(core)]];
  f ();
  ;				// { dg-error "empty statement" }
}

// A subset can be included both directly and through other definitions;
// it is still activated and suppressed as one subset.
[[subset::define(overlap, safe, no_empty_statement)]];
[[subset::define(overlap2, strict, safe, core)]];
void
use_overlap ()
{
  [[subset::activate(overlap2)]];
  ;				// { dg-error "empty statement" }
  f ();				// { dg-error "'void f\\(\\)' was subset-removed" }
  [[subset::suppress(overlap)]];
  ;
  f ();				// { dg-error "'void f\\(\\)' was subset-removed" }
  [[subset::suppress(core)]];
  [[subset::suppress(overlap2)]];	// { dg-warning "subset 'overlap2' is already suppressed" }
}

// Templates replay the expansion when instantiated.
template <typename T>
void
t ()
{
  [[subset::activate(strict)]];
  f ();				// { dg-error "'void f\\(\\)' was subset-removed" }
}
template void t<int> ();

// -Wredundant-subset warns only if nothing would change.
void
redundant ()
{
  [[subset::activate(no_empty_statement)]];
  [[subset::activate(safe)]];
  [[subset::activate(safe)]];		// { dg-warning "subset 'safe' is already active" }
  [[subset::suppress(throw_non_std_exception)]];
  [[subset::suppress(safe)]];
  [[subset::suppress(safe)]];		// { dg-warning "subset 'safe' is already suppressed" }
}

// Errors.
[[subset::define(safe, core)]];		// { dg-error "subset 'safe' is already known" }
[[subset::define(core, extra)]];	// { dg-error "subset 'core' is already known" }
[[subset::define(bad1, nonexistent)]];	// { dg-error "unknown subset 'nonexistent'" }
[[subset::define(bad2, bad2)]];		// { dg-error "unknown subset 'bad2'" }
[[subset::define(bad3, core, core)]];	// { dg-error "subset 'core' is listed more than once in the definition of 'bad3'" }
[[subset::define(bad4, core, extra, core)]];	// { dg-error "subset 'core' is listed more than once" }
[[subset::define(bad5, safe, strict, safe)]];	// { dg-error "subset 'safe' is listed more than once" }
[[subset::activate(bad3)]];		// { dg-error "unknown subset 'bad3'" }
[[subset::define]];			// { dg-error "requires a subset group name and at least one subset group" }
[[subset::define(bad6)]];		// { dg-error "requires a subset group name and at least one subset group" }
[[subset::define()]];			// { dg-error "requires subset group names" }
[[subset::define(bad7, 1)]];		// { dg-error "requires subset group names" }
[[subset::define(bad8, core extra)]];	// { dg-error "requires subset group names" }
[[subset::define(bad9, core,)]];	// { dg-error "requires subset group names" }

void
block ()
{
  [[subset::define(local, core)]];	// { dg-error "must appear at namespace scope" }
}

namespace N
{
  [[subset::define(in_namespace, core)]];
}
[[subset::activate(in_namespace)]];
[[subset::suppress(in_namespace)]];

[[subset::define(misplaced, core)]] void h ();	// { dg-warning "must appear as a separate declaration at namespace scope" }
struct S
{
  [[subset::define(member, core)]] int i;	// { dg-warning "must appear as a separate declaration at namespace scope" }
};

// Something cannot be removed under a defined subset.
[[subset::remove(safe)]] void r1 ();		// { dg-error "cannot remove under subset group 'safe', which is defined by 'subset::define'" }
[[subset::remove(core), subset::remove(strict)]] void r2 ();	// { dg-error "cannot remove under subset group 'strict'" }
