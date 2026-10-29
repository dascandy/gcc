// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
// Subset c_string removes the C string functions that std::string,
// std::string_view, std::format and their wide forms replace, but not
// strlen and wcslen.

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <string.h>
#include <strings.h>

[[subset::activate(c_string)]];

void
f (char *d, const char *s, char *m, wchar_t *wd, const wchar_t *ws, char **p,
   std::va_list ap)
{
  std::strcpy (d, s);		// { dg-error "'char\\* strcpy\\(\[^'\]*' was subset-removed" }
  std::strncpy (d, s, 1);	// { dg-error "'char\\* strncpy\\(\[^'\]*' was subset-removed" }
  std::strcat (d, s);		// { dg-error "'char\\* strcat\\(\[^'\]*' was subset-removed" }
  std::strncat (d, s, 1);	// { dg-error "'char\\* strncat\\(\[^'\]*' was subset-removed" }
  strdup (s);			// { dg-error "'char\\* strdup\\(\[^'\]*' was subset-removed" }
  strndup (s, 1);		// { dg-error "'char\\* strndup\\(\[^'\]*' was subset-removed" }
  std::strcmp (d, s);		// { dg-error "'int strcmp\\(\[^'\]*' was subset-removed" }
  std::strncmp (d, s, 1);	// { dg-error "'int strncmp\\(\[^'\]*' was subset-removed" }
  strcasecmp (d, s);		// { dg-error "'int strcasecmp\\(\[^'\]*' was subset-removed" }
  strncasecmp (d, s, 1);	// { dg-error "'int strncasecmp\\(\[^'\]*' was subset-removed" }
  std::strchr (s, 'a');		// { dg-error "'const char\\* strchr\\(\[^'\]*' was subset-removed" }
  std::strrchr (m, 'a');	// { dg-error "'char\\* strrchr\\(\[^'\]*' was subset-removed" }
  std::strstr (s, "a");		// { dg-error "'const char\\* strstr\\(\[^'\]*' was subset-removed" }
  std::strspn (s, "a");		// { dg-error "'size_t strspn\\(\[^'\]*' was subset-removed" }
  std::strcspn (s, "a");	// { dg-error "'size_t strcspn\\(\[^'\]*' was subset-removed" }
  std::strpbrk (s, "a");	// { dg-error "'const char\\* strpbrk\\(\[^'\]*' was subset-removed" }
  std::strtok (d, " ");		// { dg-error "'char\\* strtok\\(\[^'\]*' was subset-removed" }
  strtok_r (d, " ", p);		// { dg-error "'char\\* strtok_r\\(\[^'\]*' was subset-removed" }
  strsep (p, " ");		// { dg-error "'char\\* strsep\\(\[^'\]*' was subset-removed" }
  std::wcscpy (wd, ws);		// { dg-error "'wchar_t\\* wcscpy\\(\[^'\]*' was subset-removed" }
  std::wcsncpy (wd, ws, 1);	// { dg-error "'wchar_t\\* wcsncpy\\(\[^'\]*' was subset-removed" }
  std::wcscat (wd, ws);		// { dg-error "'wchar_t\\* wcscat\\(\[^'\]*' was subset-removed" }
  std::wcsncat (wd, ws, 1);	// { dg-error "'wchar_t\\* wcsncat\\(\[^'\]*' was subset-removed" }
  std::wcscmp (wd, ws);		// { dg-error "'int wcscmp\\(\[^'\]*' was subset-removed" }
  std::wcsncmp (wd, ws, 1);	// { dg-error "'int wcsncmp\\(\[^'\]*' was subset-removed" }
  std::wcschr (ws, L'a');	// { dg-error "'const wchar_t\\* wcschr\\(\[^'\]*' was subset-removed" }
  std::wcsrchr (ws, L'a');	// { dg-error "'const wchar_t\\* wcsrchr\\(\[^'\]*' was subset-removed" }
  std::wcsstr (ws, L"a");	// { dg-error "'const wchar_t\\* wcsstr\\(\[^'\]*' was subset-removed" }
  std::wcsspn (ws, L"a");	// { dg-error "'size_t wcsspn\\(\[^'\]*' was subset-removed" }
  std::wcscspn (ws, L"a");	// { dg-error "'size_t wcscspn\\(\[^'\]*' was subset-removed" }
  std::wcspbrk (ws, L"a");	// { dg-error "'const wchar_t\\* wcspbrk\\(\[^'\]*' was subset-removed" }
  std::wcstok (wd, L" ", &wd);	// { dg-error "'wchar_t\\* wcstok\\(\[^'\]*' was subset-removed" }
  stpcpy (d, s);		// { dg-error "'char\\* stpcpy\\(\[^'\]*' was subset-removed" }
  stpncpy (d, s, 1);		// { dg-error "'char\\* stpncpy\\(\[^'\]*' was subset-removed" }
  strchrnul (s, 'a');		// { dg-error "'const char\\* strchrnul\\(\[^'\]*' was subset-removed" }
  strcasestr (s, "a");		// { dg-error "'const char\\* strcasestr\\(\[^'\]*' was subset-removed" }
  index (s, 'a');		// { dg-error "'const char\\* index\\(\[^'\]*' was subset-removed" }
  rindex (m, 'a');		// { dg-error "'char\\* rindex\\(\[^'\]*' was subset-removed" }
  std::sprintf (d, "a");	// { dg-error "'int sprintf\\(\[^'\]*' was subset-removed" }
  std::vsprintf (d, s, ap);	// { dg-error "'int vsprintf\\(\[^'\]*' was subset-removed" }

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
  _stricmp (s, s);		// { dg-error "'int _stricmp\\(\[^'\]*' was subset-removed" }
  _strnicmp (s, s, 1);		// { dg-error "'int _strnicmp\\(\[^'\]*' was subset-removed" }
  {
    [[subset::suppress(c_string)]];
    _stricmp (s, s);
  }
}

void
h (const char *s)
{
  [[subset::suppress(c_string), subset::activate(unicode)]];
  _stricmp (s, s);		// { dg-error "'int _stricmp\\(\[^'\]*' was subset-removed" }
  strcasecmp (s, s);		// { dg-error "'int strcasecmp\\(\[^'\]*' was subset-removed" }
  _strnicmp (s, s, 1);		// { dg-error "'int _strnicmp\\(\[^'\]*' was subset-removed" }
  strncasecmp (s, s, 1);	// { dg-error "'int strncasecmp\\(\[^'\]*' was subset-removed" }
  std::strchr (s, 'a');
}
