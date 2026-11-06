// { dg-do compile { target c++23 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR -Wno-deprecated-declarations" }
// Policy unicode removes the narrow output functions that std::print
// replaces, std::vprint_nonunicode and std::vprint_nonunicode_buffered, the
// functions that cut text at a number of bytes, and the members of
// std::filesystem::path that give a narrow string in the native encoding.

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <ostream>
#include <print>

[[policy::activate(unicode)]];

void
f (char *d, const char *s, std::va_list ap, const std::filesystem::path &p,
   std::ostream &os)
{
  std::vprintf (s, ap);		// { dg-error "'int vprintf\\(\[^'\]*' was policy-removed" }
  std::fprintf (stdout, "a");	// { dg-error "'int fprintf\\(\[^'\]*' was policy-removed" }
  std::vfprintf (stdout, s, ap);	// { dg-error "'int vfprintf\\(\[^'\]*' was policy-removed" }
  std::fputs ("a", stdout);	// { dg-error "'int fputs\\(\[^'\]*' was policy-removed" }
  std::putchar ('a');		// { dg-error "'int putchar\\(int\\)' was policy-removed" }
  std::putc ('a', stdout);	// { dg-error "'int putc\\(\[^'\]*' was policy-removed" }
  std::fputc ('a', stdout);	// { dg-error "'int fputc\\(\[^'\]*' was policy-removed" }
  std::vsnprintf (d, 1, s, ap);	// { dg-error "'int vsnprintf\\(\[^'\]*' was policy-removed" }
  strndup (s, 1);		// { dg-error "'char\\* strndup\\(\[^'\]*' was policy-removed" }
  std::fgets (d, 1, stdin);	// { dg-error "'char\\* fgets\\(\[^'\]*' was policy-removed" }
  std::vprint_nonunicode (stdout, "a", std::make_format_args ());	// { dg-error "'void std::vprint_nonunicode\\(FILE\\*\[^'\]*' was policy-removed" }
  std::vprint_nonunicode_buffered (stdout, "a", std::make_format_args ());	// { dg-error "'void std::vprint_nonunicode_buffered\\(FILE\\*\[^'\]*' was policy-removed" }
  std::vprint_nonunicode_buffered ("a", std::make_format_args ());	// { dg-error "'void std::vprint_nonunicode_buffered\\(\[^,'\]*string_view\[^'\]*' was policy-removed" }
  std::vprint_nonunicode (os, "a", std::make_format_args ());	// { dg-error "'void std::vprint_nonunicode\\(\[^,'\]*ostream&\[^'\]*' was policy-removed" }
  (void) p.string ();		// { dg-error "path::string\\(\\) const'\[^\n\]*was policy-removed" }
  (void) p.generic_string ();	// { dg-error "path::generic_string\\(\\) const'\[^\n\]*was policy-removed" }
#if __glibcxx_format_path >= 202506L
  (void) p.native_encoded_string ();	// { dg-error "path::native_encoded_string\\(\\) const'\[^\n\]*was policy-removed" "" { target c++26 } }
  (void) p.generic_native_encoded_string ();	// { dg-error "path::generic_native_encoded_string\\(\\) const'\[^\n\]*was policy-removed" "" { target c++26 } }
  (void) p.display_string ();
  (void) p.generic_display_string ();
#endif

  // Not removed.
  std::print ("a");
  std::println (stderr, "a");
  std::print (os, "a");
  (void) p.u8string ();
  (void) p.generic_u8string ();
  (void) p.native ();
  std::sprintf (d, "a");
  std::fwrite (s, 1, 1, stdout);
  std::fflush (stdout);
}
