// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
// Policy c_string removes the C string functions that std::string,
// std::string_view, std::format and their wide forms replace, but not
// strlen and wcslen.

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <string.h>
#include <strings.h>

[[policy::activate(c_string)]];

void
f (char *d, const char *s, char *m, wchar_t *wd, const wchar_t *ws, char **p,
   std::va_list ap)
{
  std::strcpy (d, s);		// { dg-error "'char\\* strcpy\\(\[^'\]*' was policy-removed" }
  std::strncpy (d, s, 1);	// { dg-error "'char\\* strncpy\\(\[^'\]*' was policy-removed" }
  std::strcat (d, s);		// { dg-error "'char\\* strcat\\(\[^'\]*' was policy-removed" }
  std::strncat (d, s, 1);	// { dg-error "'char\\* strncat\\(\[^'\]*' was policy-removed" }
  strdup (s);			// { dg-error "'char\\* strdup\\(\[^'\]*' was policy-removed" }
  strndup (s, 1);		// { dg-error "'char\\* strndup\\(\[^'\]*' was policy-removed" }
  std::strcmp (d, s);		// { dg-error "'int strcmp\\(\[^'\]*' was policy-removed" }
  std::strncmp (d, s, 1);	// { dg-error "'int strncmp\\(\[^'\]*' was policy-removed" }
  strcasecmp (d, s);		// { dg-error "'int strcasecmp\\(\[^'\]*' was policy-removed" }
  strncasecmp (d, s, 1);	// { dg-error "'int strncasecmp\\(\[^'\]*' was policy-removed" }
  std::strchr (s, 'a');		// { dg-error "'const char\\* strchr\\(\[^'\]*' was policy-removed" }
  std::strrchr (m, 'a');	// { dg-error "'char\\* strrchr\\(\[^'\]*' was policy-removed" }
  std::strstr (s, "a");		// { dg-error "'const char\\* strstr\\(\[^'\]*' was policy-removed" }
  std::strspn (s, "a");		// { dg-error "'size_t strspn\\(\[^'\]*' was policy-removed" }
  std::strcspn (s, "a");	// { dg-error "'size_t strcspn\\(\[^'\]*' was policy-removed" }
  std::strpbrk (s, "a");	// { dg-error "'const char\\* strpbrk\\(\[^'\]*' was policy-removed" }
  std::strtok (d, " ");		// { dg-error "'char\\* strtok\\(\[^'\]*' was policy-removed" }
  strtok_r (d, " ", p);		// { dg-error "'char\\* strtok_r\\(\[^'\]*' was policy-removed" }
  strsep (p, " ");		// { dg-error "'char\\* strsep\\(\[^'\]*' was policy-removed" }
  std::wcscpy (wd, ws);		// { dg-error "'wchar_t\\* wcscpy\\(\[^'\]*' was policy-removed" }
  std::wcsncpy (wd, ws, 1);	// { dg-error "'wchar_t\\* wcsncpy\\(\[^'\]*' was policy-removed" }
  std::wcscat (wd, ws);		// { dg-error "'wchar_t\\* wcscat\\(\[^'\]*' was policy-removed" }
  std::wcsncat (wd, ws, 1);	// { dg-error "'wchar_t\\* wcsncat\\(\[^'\]*' was policy-removed" }
  std::wcscmp (wd, ws);		// { dg-error "'int wcscmp\\(\[^'\]*' was policy-removed" }
  std::wcsncmp (wd, ws, 1);	// { dg-error "'int wcsncmp\\(\[^'\]*' was policy-removed" }
  std::wcschr (ws, L'a');	// { dg-error "'const wchar_t\\* wcschr\\(\[^'\]*' was policy-removed" }
  std::wcsrchr (ws, L'a');	// { dg-error "'const wchar_t\\* wcsrchr\\(\[^'\]*' was policy-removed" }
  std::wcsstr (ws, L"a");	// { dg-error "'const wchar_t\\* wcsstr\\(\[^'\]*' was policy-removed" }
  std::wcsspn (ws, L"a");	// { dg-error "'size_t wcsspn\\(\[^'\]*' was policy-removed" }
  std::wcscspn (ws, L"a");	// { dg-error "'size_t wcscspn\\(\[^'\]*' was policy-removed" }
  std::wcspbrk (ws, L"a");	// { dg-error "'const wchar_t\\* wcspbrk\\(\[^'\]*' was policy-removed" }
  std::wcstok (wd, L" ", &wd);	// { dg-error "'wchar_t\\* wcstok\\(\[^'\]*' was policy-removed" }
  stpcpy (d, s);		// { dg-error "'char\\* stpcpy\\(\[^'\]*' was policy-removed" }
  stpncpy (d, s, 1);		// { dg-error "'char\\* stpncpy\\(\[^'\]*' was policy-removed" }
  strchrnul (s, 'a');		// { dg-error "'const char\\* strchrnul\\(\[^'\]*' was policy-removed" }
  strcasestr (s, "a");		// { dg-error "'const char\\* strcasestr\\(\[^'\]*' was policy-removed" }
  index (s, 'a');		// { dg-error "'const char\\* index\\(\[^'\]*' was policy-removed" }
  rindex (m, 'a');		// { dg-error "'char\\* rindex\\(\[^'\]*' was policy-removed" }
  std::sprintf (d, "a");	// { dg-error "'int sprintf\\(\[^'\]*' was policy-removed" }
  std::vsprintf (d, s, ap);	// { dg-error "'int vsprintf\\(\[^'\]*' was policy-removed" }

  std::strlen (s);
  std::wcslen (ws);
  std::memcpy (d, s, 1);
  std::strerror (0);
  std::snprintf (d, 1, "a");
}

// _stricmp and _strnicmp are not declared by glibc.
extern "C" int _stricmp (const char *, const char *);
extern "C" int _strnicmp (const char *, const char *, unsigned long);

void
g (const char *s)
{
  _stricmp (s, s);		// { dg-error "'int _stricmp\\(\[^'\]*' was policy-removed" }
  _strnicmp (s, s, 1);		// { dg-error "'int _strnicmp\\(\[^'\]*' was policy-removed" }
  {
    [[policy::suppress(c_string)]];
    _stricmp (s, s);
  }
}

void
h (const char *s)
{
  [[policy::suppress(c_string), policy::activate(unicode)]];
  _stricmp (s, s);		// { dg-error "'int _stricmp\\(\[^'\]*' was policy-removed" }
  strcasecmp (s, s);		// { dg-error "'int strcasecmp\\(\[^'\]*' was policy-removed" }
  _strnicmp (s, s, 1);		// { dg-error "'int _strnicmp\\(\[^'\]*' was policy-removed" }
  strncasecmp (s, s, 1);	// { dg-error "'int strncasecmp\\(\[^'\]*' was policy-removed" }
  std::strchr (s, 'a');
}
