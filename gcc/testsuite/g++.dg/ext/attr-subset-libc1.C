// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
// The C library functions in the compiler's table are removed under a
// subset group of their own name, however their header declares them.

#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <string.h>

void
f (char *d, const char *s, char *m)
{
  [[subset::activate(fopen), subset::activate(strcpy),
    subset::activate(strstr), subset::activate(strchr),
    subset::activate(printf)]];
  std::fopen ("a", "r");	// { dg-error "'FILE\\* fopen\\(const char\\*, const char\\*\\)' was subset-removed" }
  fopen ("a", "r");		// { dg-error "'FILE\\* fopen\\(const char\\*, const char\\*\\)' was subset-removed" }
  std::strcpy (d, s);		// { dg-error "'char\\* strcpy\\(char\\*, const char\\*\\)' was subset-removed" }
  strstr (s, "x");		// { dg-error "'const char\\* strstr\\(const char\\*, const char\\*\\)' was subset-removed" }
  strchr (s, 'x');		// { dg-error "'const char\\* strchr\\(const char\\*, int\\)' was subset-removed" }
  strchr (m, 'x');		// { dg-error "'char\\* strchr\\(char\\*, int\\)' was subset-removed" }
  std::strchr (s, 'x');		// { dg-error "'const char\\* strchr\\(const char\\*, int\\)' was subset-removed" }
  printf ("x");			// { dg-error "'int printf\\(const char\\*, ...\\)' was subset-removed" }
  std::puts ("x");
  std::strlen (s);
  std::fclose (nullptr);
}

void
g (char *d, const char *s)
{
  fopen ("a", "r");
  strcpy (d, s);
  printf ("x");
}
