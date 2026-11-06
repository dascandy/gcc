// { dg-do compile { target c++11 } }
// The functions of Annex K of the C standard, and strlcpy and strlcat, are
// in the predefined policies of the functions they are safer forms of.
// glibc does not declare all of them, so they are declared here.

typedef int errno_t;
typedef __SIZE_TYPE__ rsize_t;
typedef __SIZE_TYPE__ size_t;
struct FILE;

extern "C" {
errno_t fopen_s (FILE **, const char *, const char *);
errno_t freopen_s (FILE **, const char *, const char *, FILE *);
char *gets_s (char *, rsize_t);
int printf_s (const char *, ...);
int fprintf_s (FILE *, const char *, ...);
int sprintf_s (char *, rsize_t, const char *, ...);
int snprintf_s (char *, rsize_t, const char *, ...);
errno_t strcpy_s (char *, rsize_t, const char *);
errno_t strncpy_s (char *, rsize_t, const char *, rsize_t);
errno_t strcat_s (char *, rsize_t, const char *);
errno_t strncat_s (char *, rsize_t, const char *, rsize_t);
char *strtok_s (char *, rsize_t *, const char *, char **);
char *_strdup (const char *);
size_t strlcpy (char *, const char *, size_t);
size_t strlcat (char *, const char *, size_t);
int wprintf_s (const wchar_t *, ...);
int wscanf_s (const wchar_t *, ...);
errno_t wcscpy_s (wchar_t *, rsize_t, const wchar_t *);
}

void
f (FILE **fp, char *d, const char *s, char **p, wchar_t *wd, rsize_t *n)
{
  [[policy::activate(unicode)]];
  fopen_s (fp, "a", "r");	// { dg-error "'errno_t fopen_s\\(\[^'\]*' was policy-removed" }
  freopen_s (fp, "a", "r", *fp);	// { dg-error "'errno_t freopen_s\\(\[^'\]*' was policy-removed" }
  gets_s (d, 1);		// { dg-error "'char\\* gets_s\\(\[^'\]*' was policy-removed" }
  printf_s ("a");		// { dg-error "'int printf_s\\(\[^'\]*' was policy-removed" }
  fprintf_s (*fp, "a");		// { dg-error "'int fprintf_s\\(\[^'\]*' was policy-removed" }
  snprintf_s (d, 1, "a");	// { dg-error "'int snprintf_s\\(\[^'\]*' was policy-removed" }
  strncpy_s (d, 1, s, 1);	// { dg-error "'errno_t strncpy_s\\(\[^'\]*' was policy-removed" }
  wprintf_s (L"a");		// { dg-error "'int wprintf_s\\(\[^'\]*' was policy-removed" }
  wscanf_s (L"a");		// { dg-error "'int wscanf_s\\(\[^'\]*' was policy-removed" }
  strcpy_s (d, 1, s);
  sprintf_s (d, 1, "a");
  strlcpy (d, s, 1);
}

void
g (char *d, const char *s, char **p, rsize_t *n)
{
  [[policy::activate(c_string)]];
  strcpy_s (d, 1, s);		// { dg-error "'errno_t strcpy_s\\(\[^'\]*' was policy-removed" }
  strncpy_s (d, 1, s, 1);	// { dg-error "'errno_t strncpy_s\\(\[^'\]*' was policy-removed" }
  strcat_s (d, 1, s);		// { dg-error "'errno_t strcat_s\\(\[^'\]*' was policy-removed" }
  strncat_s (d, 1, s, 1);	// { dg-error "'errno_t strncat_s\\(\[^'\]*' was policy-removed" }
  strtok_s (d, n, " ", p);	// { dg-error "'char\\* strtok_s\\(\[^'\]*' was policy-removed" }
  _strdup (s);			// { dg-error "'char\\* _strdup\\(\[^'\]*' was policy-removed" }
  strlcpy (d, s, 1);		// { dg-error "'size_t strlcpy\\(\[^'\]*' was policy-removed" }
  strlcat (d, s, 1);		// { dg-error "'size_t strlcat\\(\[^'\]*' was policy-removed" }
  sprintf_s (d, 1, "a");	// { dg-error "'int sprintf_s\\(\[^'\]*' was policy-removed" }
  printf_s ("a");
}

