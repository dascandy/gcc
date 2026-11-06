// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
// The C library functions in the compiler's table are removed under a
// policy group of their own name, however their header declares them.

#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <string.h>

void
f (char *d, const char *s, char *m)
{
  [[policy::activate(fopen), policy::activate(strcpy),
    policy::activate(strstr), policy::activate(strchr),
    policy::activate(printf)]];
  std::fopen ("a", "r");	// { dg-error "'FILE\\* fopen\\(const char\\*, const char\\*\\)' was policy-removed" }
  fopen ("a", "r");		// { dg-error "'FILE\\* fopen\\(const char\\*, const char\\*\\)' was policy-removed" }
  std::strcpy (d, s);		// { dg-error "'char\\* strcpy\\(char\\*, const char\\*\\)' was policy-removed" }
  strstr (s, "x");		// { dg-error "'const char\\* strstr\\(const char\\*, const char\\*\\)' was policy-removed" }
  strchr (s, 'x');		// { dg-error "'const char\\* strchr\\(const char\\*, int\\)' was policy-removed" }
  strchr (m, 'x');		// { dg-error "'char\\* strchr\\(char\\*, int\\)' was policy-removed" }
  std::strchr (s, 'x');		// { dg-error "'const char\\* strchr\\(const char\\*, int\\)' was policy-removed" }
  printf ("x");			// { dg-error "'int printf\\(const char\\*, ...\\)' was policy-removed" }
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
