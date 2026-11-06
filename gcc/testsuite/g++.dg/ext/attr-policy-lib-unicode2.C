// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
// Policy unicode removes the wide streams, the wide output and input
// functions, freopen, remove and rename, snprintf and the string functions
// that cut text at a number of bytes or fold case as ASCII, and the
// <cctype> functions that classify or convert letters, but not the
// templates of the same name, nor the functions in unicode_todo.md.

#include <iostream>
#include <algorithm>
#include <locale>
#include <cctype>
#include <cwctype>
#include <cstdio>
#include <cwchar>
#include <cstring>
#include <strings.h>

[[policy::activate(unicode)]];

void
f (int c, wchar_t *ws, int *b, int *e, char *d, const char *s)
{
  std::wcout << L"a";		// { dg-error "'std::wcout' was policy-removed" }
  std::wcerr << L"a";		// { dg-error "'std::wcerr' was policy-removed" }
  std::wclog << L"a";		// { dg-error "'std::wclog' was policy-removed" }
  std::wcin >> *ws;		// { dg-error "'std::wcin' was policy-removed" }
  std::wprintf (L"a");		// { dg-error "wprintf\\(const wchar_t\\*, ...\\)' was policy-removed" }
  std::fwprintf (stdout, L"a");	// { dg-error "'int fwprintf\\(\[^'\]*' was policy-removed" }
  std::fputws (L"a", stdout);	// { dg-error "'int fputws\\(\[^'\]*' was policy-removed" }
  std::putwchar (L'a');		// { dg-error "putwchar\\(wchar_t\\)' was policy-removed" }
  std::fgetws (ws, 1, stdin);	// { dg-error "'wchar_t\\* fgetws\\(\[^'\]*' was policy-removed" }
  std::getwchar ();		// { dg-error "getwchar\\(\\)' was policy-removed" }
  std::wscanf (L"%d", &c);	// { dg-error "wscanf\\(const wchar_t\\*, ...\\)' was policy-removed" }
  std::remove ("a");		// { dg-error "'int remove\\(const char\\*\\)' was policy-removed" }
  std::rename ("a", "b");	// { dg-error "'int rename\\(const char\\*, const char\\*\\)' was policy-removed" }
  std::isalpha (c);		// { dg-error "'int isalpha\\(int\\)' was policy-removed" }
  std::isupper (c);		// { dg-error "'int isupper\\(int\\)' was policy-removed" }
  std::isprint (c);		// { dg-error "'int isprint\\(int\\)' was policy-removed" }
  std::toupper (c);		// { dg-error "'int toupper\\(int\\)' was policy-removed" }
  tolower (c);			// { dg-error "'int tolower\\(int\\)' was policy-removed" }
  std::freopen ("a", "r", stdin);	// { dg-error "'\[^'\]* freopen\\(\[^'\]*' was policy-removed" }
  std::snprintf (d, 1, "a");	// { dg-error "'int snprintf\\(\[^'\]*' was policy-removed" }
  std::strncpy (d, s, 1);	// { dg-error "'char\\* strncpy\\(\[^'\]*' was policy-removed" }
  std::strncat (d, s, 1);	// { dg-error "'char\\* strncat\\(\[^'\]*' was policy-removed" }
  strcasecmp (d, s);		// { dg-error "'int strcasecmp\\(\[^'\]*' was policy-removed" }

  // Not removed.
  (void) std::remove (b, e, 0);
  std::isalpha ('a', std::locale ());
  std::toupper ('a', std::locale ());
  std::isdigit (c);
  std::isspace (c);
  std::towupper (L'a');
  std::fflush (stdout);
  std::strcpy (d, s);
  std::strcmp (d, s);
  std::endl (std::cout);	// { dg-error "'std::cout' was policy-removed" }
}
