// { dg-do compile { target c++17 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
// Subset fstream_narrow_name removes the members of the file streams that
// take a file name as a const char * or a std::string, but not those that
// take a std::filesystem::path.  It is a member of subset unicode.

#include <fstream>
#include <filesystem>
#include <string>

void
f (const std::string &s, const std::filesystem::path &p)
{
  [[subset::activate(unicode)]];
  std::ifstream i1 ("a");	// { dg-error "basic_ifstream\\(const char\\*\[^'\]*' was subset-removed" }
  std::ifstream i2 (s);		// { dg-error "basic_ifstream\\(const std::\[^'\]*' was subset-removed" }
  std::ofstream o1 ("a");	// { dg-error "basic_ofstream\\(const char\\*\[^'\]*' was subset-removed" }
  std::ofstream o2 (s);		// { dg-error "basic_ofstream\\(const std::\[^'\]*' was subset-removed" }
  std::fstream f1 ("a");	// { dg-error "basic_fstream\\(const char\\*\[^'\]*' was subset-removed" }
  std::fstream f2 (s);		// { dg-error "basic_fstream\\(const std::\[^'\]*' was subset-removed" }
  std::wifstream w1 ("a");	// { dg-error "basic_ifstream\\(const char\\*\[^'\]*' was subset-removed" }
  std::filebuf b;
  b.open ("a", std::ios_base::in);	// { dg-error "::open\\(const char\\*\[^'\]*' was subset-removed" }
  b.open (s, std::ios_base::in);	// { dg-error "::open\\(const std::\[^'\]*' was subset-removed" }

  std::ifstream i3 (p);
  std::ofstream o3 (p);
  std::fstream f3 (p);
  std::ifstream i4;
  i4.open ("a");		// { dg-error "::open\\(const char\\*\[^'\]*' was subset-removed" }
  i4.open (s);			// { dg-error "::open\\(const std::\[^'\]*' was subset-removed" }
  i4.open (p);
  std::ofstream o4;
  o4.open ("a");		// { dg-error "::open\\(const char\\*\[^'\]*' was subset-removed" }
  o4.open (p);
  std::fstream f4;
  f4.open (s);			// { dg-error "::open\\(const std::\[^'\]*' was subset-removed" }
  f4.open (p);
  b.open (p, std::ios_base::in);
  std::ifstream i5 (std::filesystem::path (u8"a"));
}

void
g (const std::string &s)
{
  std::ifstream i1 ("a");
  std::ofstream o1 (s);
}
