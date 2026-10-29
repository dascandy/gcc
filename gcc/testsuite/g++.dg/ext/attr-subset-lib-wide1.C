// { dg-do compile { target c++23 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR -Wno-deprecated-declarations" }
// Subset wide_string removes wchar_t and its literals, and what the C and
// C++ libraries have for wide characters and strings.

#include <string>
#include <string_view>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <syncstream>
#include <spanstream>
#include <regex>
#include <format>
#include <cwchar>
#include <cwctype>
#include <cstdlib>
#include <cstring>
#include <cinttypes>
#include <locale>
#include <codecvt>

[[subset::activate(wide_string)]];

void
f (const std::filesystem::path &p)
{
  std::wstring a;	// { dg-error "'std::wstring'\[^\n\]*was subset-removed" }
  std::pmr::wstring b;	// { dg-error "'std::pmr::wstring'\[^\n\]*was subset-removed" }
  std::wstring_view c;	// { dg-error "'std::wstring_view'\[^\n\]*was subset-removed" }
  auto d = std::to_wstring (1);	// { dg-error "std::wstring std::__cxx11::to_wstring\\(int\\)'\[^\n\]*was subset-removed" }
  auto e = p.wstring ();	// { dg-error "'std::wstring std::filesystem::__cxx11::path::wstring\\(\\) const'\[^\n\]*was subset-removed" }
  auto e2 = p.generic_wstring ();	// { dg-error "'std::wstring std::filesystem::__cxx11::path::generic_wstring\\(\\) const'\[^\n\]*was subset-removed" }
  std::wios *g;	// { dg-error "'std::wios'\[^\n\]*was subset-removed" }
  std::wstreambuf *h;	// { dg-error "'std::wstreambuf'\[^\n\]*was subset-removed" }
  std::wistream *i;	// { dg-error "'std::wistream'\[^\n\]*was subset-removed" }
  std::wostream *j;	// { dg-error "'std::wostream'\[^\n\]*was subset-removed" }
  std::wiostream *k;	// { dg-error "'std::wiostream'\[^\n\]*was subset-removed" }
  std::wstringbuf *l;	// { dg-error "'std::wstringbuf'\[^\n\]*was subset-removed" }
  std::wistringstream *m;	// { dg-error "'std::wistringstream'\[^\n\]*was subset-removed" }
  std::wostringstream *n;	// { dg-error "'std::wostringstream'\[^\n\]*was subset-removed" }
  std::wstringstream *o;	// { dg-error "'std::wstringstream'\[^\n\]*was subset-removed" }
  std::wfilebuf *q;	// { dg-error "'std::wfilebuf'\[^\n\]*was subset-removed" }
  std::wifstream *r;	// { dg-error "'std::wifstream'\[^\n\]*was subset-removed" }
  std::wofstream *s;	// { dg-error "'std::wofstream'\[^\n\]*was subset-removed" }
  std::wfstream *t;	// { dg-error "'std::wfstream'\[^\n\]*was subset-removed" }
  std::wsyncbuf *u;	// { dg-error "'std::wsyncbuf'\[^\n\]*was subset-removed" }
  std::wosyncstream *v;	// { dg-error "'std::wosyncstream'\[^\n\]*was subset-removed" }
  std::wspanbuf *w;	// { dg-error "'std::wspanbuf'\[^\n\]*was subset-removed" }
  std::wispanstream *x;	// { dg-error "'std::wispanstream'\[^\n\]*was subset-removed" }
  std::wospanstream *y;	// { dg-error "'std::wospanstream'\[^\n\]*was subset-removed" }
  std::wspanstream *z;	// { dg-error "'std::wspanstream'\[^\n\]*was subset-removed" }
  std::wstreampos *aa;	// { dg-error "'std::wstreampos'\[^\n\]*was subset-removed" }
  std::wcout.flush ();	// { dg-error "'std::wcout'\[^\n\]*was subset-removed" }
  std::wregex *ab;	// { dg-error "'std::__cxx11::wregex'\[^\n\]*was subset-removed" }
  std::wcsub_match *ac;	// { dg-error "'std::__cxx11::wcsub_match'\[^\n\]*was subset-removed" }
  std::wssub_match *ad;	// { dg-error "'std::__cxx11::wssub_match'\[^\n\]*was subset-removed" }
  std::wcmatch *ae;	// { dg-error "'std::__cxx11::wcmatch'\[^\n\]*was subset-removed" }
  std::wsmatch *af;	// { dg-error "'std::__cxx11::wsmatch'\[^\n\]*was subset-removed" }
  std::pmr::wcmatch *ag;	// { dg-error "'std::pmr::wcmatch'\[^\n\]*was subset-removed" }
  std::wcregex_iterator *ah;	// { dg-error "'std::__cxx11::wcregex_iterator'\[^\n\]*was subset-removed" }
  std::wsregex_iterator *ai;	// { dg-error "'std::__cxx11::wsregex_iterator'\[^\n\]*was subset-removed" }
  std::wcregex_token_iterator *aj;	// { dg-error "'std::__cxx11::wcregex_token_iterator'\[^\n\]*was subset-removed" }
  std::wsregex_token_iterator *ak;	// { dg-error "'std::__cxx11::wsregex_token_iterator'\[^\n\]*was subset-removed" }
  std::wformat_context *al;	// { dg-error "'std::wformat_context'\[^\n\]*was subset-removed" }
  std::wformat_args *am;	// { dg-error "'std::wformat_args'\[^\n\]*was subset-removed" }
  std::wformat_parse_context *an;	// { dg-error "'std::wformat_parse_context'\[^\n\]*was subset-removed" }
  std::wformat_string<int> *ao;	// { dg-error "'std::wformat_string<int>'\[^\n\]*was subset-removed" }
  std::wcslen (d.c_str ());	// { dg-error "'size_t wcslen\\(const wchar_t\\*\\)'\[^\n\]*was subset-removed" }
  std::wmemset (nullptr, 0, 0);	// { dg-error "'wchar_t\\* wmemset\\(wchar_t\\*, wchar_t, size_t\\)'\[^\n\]*was subset-removed" }
  std::iswalpha (0);	// { dg-error "'int iswalpha\\(wint_t\\)'\[^\n\]*was subset-removed" }
  std::towupper (0);	// { dg-error "'wint_t towupper\\(wint_t\\)'\[^\n\]*was subset-removed" }
  std::btowc (0);	// { dg-error "'wint_t btowc\\(int\\)'\[^\n\]*was subset-removed" }
  std::wcstombs (nullptr, nullptr, 0);	// { dg-error "'size_t wcstombs\\(char\\*, const wchar_t\\*, size_t\\)'\[^\n\]*was subset-removed" }
  std::wcstoimax (nullptr, nullptr, 0);	// { dg-error "'intmax_t wcstoimax\\(\[^\n\]*was subset-removed" }
  std::wcstoumax (nullptr, nullptr, 0);	// { dg-error "'uintmax_t wcstoumax\\(\[^\n\]*was subset-removed" }
  int fa = 0;
  (void) std::make_wformat_args (fa);	// { dg-error "make_wformat_args\[^\n\]*was subset-removed" }
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> *ar;	// { dg-error "wstring_convert<\[^\n\]*was subset-removed" }
  std::wbuffer_convert<std::codecvt_utf8<char32_t>, char32_t> *as;	// { dg-error "wbuffer_convert<\[^\n\]*was subset-removed" }
  std::fwide (stdout, 0);	// { dg-error "'int fwide\\(__FILE\\*, int\\)'\[^\n\]*was subset-removed" }
  wchar_t ap;	// { dg-error "use of 'wchar_t'" }
  (void) L"x";	// { dg-error "wide string literal" }

  std::string ba;
  std::string_view bb;
  std::cout.flush ();
  std::strlen ("x");
  (void) std::to_string (1);
  (void) std::make_format_args (fa);
  (void) p.native ();
}
