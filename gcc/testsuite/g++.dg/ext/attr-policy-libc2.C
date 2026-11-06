// { dg-do compile { target c++11 } }
// Only the functions of the C library are removed: those with C linkage in
// the global namespace, not other functions with the same name.

extern "C" int printf (const char *, ...);
extern "C" int _stricmp (const char *, const char *);
struct S {};
void fopen (S);
namespace N { void strcpy (); }
struct C { static void printf (); };

[[policy::activate(printf)]];
[[policy::activate(_stricmp)]];
[[policy::activate(fopen)]];		// { dg-warning "unknown policy 'fopen'" }
[[policy::activate(strcpy)]];		// { dg-warning "unknown policy 'strcpy'" }

void
f ()
{
  printf ("x");			// { dg-error "'int printf\\(const char\\*, ...\\)' was policy-removed" }
  _stricmp ("x", "y");		// { dg-error "'int _stricmp\\(const char\\*, const char\\*\\)' was policy-removed" }
  fopen (S ());
  N::strcpy ();
  C::printf ();
}

void
g ()
{
  extern int printf (const char *, ...);
  printf ("y");			// { dg-error "'int printf\\(const char\\*, ...\\)' was policy-removed" }
}
