// { dg-do compile { target c++17 } }
// An empty statement is an error when subset no_empty_statement is active.

void
inactive (int x)
{
  ;
  if (x) ;
}

void
active (int x)
{
  [[subset::activate(no_empty_statement)]];
  ;				// { dg-error "empty statement" }
  if (x) ;			// { dg-error "empty statement" }
  if (x) {} else ;		// { dg-error "empty statement" }
  while (x--) ;			// { dg-error "empty statement" }
  for (;;) ;			// { dg-error "empty statement" }
  switch (x) { case 0: ; default: ; }
  {
    [[subset::suppress(no_empty_statement)]];
    ;
  }
  ;				// { dg-error "empty statement" }

  // Not empty statements.
  for (;;) {}
  for (int i = 0; ; ) {}
  {}
  x = 1;
  switch (x) { case 0: [[fallthrough]]; default: break; }
  [[subset::activate(throw_non_std_exception)]];
}

// At namespace scope, from the definition of a template.
[[subset::activate(no_empty_statement)]];
template <typename T> void t () { ; }	// { dg-error "empty statement" }
struct S { void f () { ; } };		// { dg-error "empty statement" }
;

[[subset::suppress(no_empty_statement)]];

// Not after a label, or after a macro that expands to nothing.
#define NOTHING
#define LOG(x)
#define NESTED NOTHING
#define SEMI ;			// { dg-error "empty statement" }
void
labels_and_macros (int x)
{
  [[subset::activate(no_empty_statement)]];
 l:;
  NOTHING;
  LOG (x);
  NESTED;
  if (x) LOG (x);
  while (x--) NOTHING;
  for (;;) LOG (x);
  NOTHING ;
  SEMI
  LOG (x) x = 1;
  ;				// { dg-error "empty statement" }
  if (x) goto l;
}
