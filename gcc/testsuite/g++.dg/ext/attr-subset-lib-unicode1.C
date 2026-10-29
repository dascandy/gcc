// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
// Subset unicode removes std::cout, std::cerr and std::basic_regex, and
// can be activated before the headers that remove them are included.
[[subset::activate(unicode)]];

#include <iostream>
#include <regex>

void
f ()
{
  std::cout << "a\n";		// { dg-error "'std::cout' was subset-removed" }
  std::cerr << "b\n";		// { dg-error "'std::cerr' was subset-removed" }
  std::clog << "c\n";
  std::regex r ("x");		// { dg-error "basic_regex<char>' was subset-removed" }
  std::wregex w (L"x");		// { dg-error "basic_regex<wchar_t>' was subset-removed" }
  std::clog << std::endl;
}

void
g ()
{
  [[subset::suppress(cerr)]];
  std::cerr << "b\n";
  std::cout << "a\n";		// { dg-error "'std::cout' was subset-removed" }
}
