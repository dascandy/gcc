// { dg-do compile { target c++11 } }
// Only the functions of the C library are removed: those with C linkage in
// the global namespace, not other functions with the same name.

extern "C" int printf (const char *, ...);
extern "C" int _stricmp (const char *, const char *);
struct S {};
void fopen (S);
namespace N { void strcpy (); }
struct C { static void printf (); };

[[subset::activate(printf)]];
[[subset::activate(_stricmp)]];
[[subset::activate(fopen)]];		// { dg-warning "unknown subset 'fopen'" }
[[subset::activate(strcpy)]];		// { dg-warning "unknown subset 'strcpy'" }

void
f ()
{
  printf ("x");			// { dg-error "'int printf\\(const char\\*, ...\\)' was subset-removed" }
  _stricmp ("x", "y");		// { dg-error "'int _stricmp\\(const char\\*, const char\\*\\)' was subset-removed" }
  fopen (S ());
  N::strcpy ();
  C::printf ();
}

void
g ()
{
  extern int printf (const char *, ...);
  printf ("y");			// { dg-error "'int printf\\(const char\\*, ...\\)' was subset-removed" }
}
