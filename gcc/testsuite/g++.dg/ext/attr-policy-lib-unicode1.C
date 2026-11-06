// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
// Policy unicode removes std::cout, std::cerr and std::basic_regex, and
// can be activated before the headers that remove them are included.
[[policy::activate(unicode)]];

#include <iostream>
#include <regex>

void
f ()
{
  std::cout << "a\n";		// { dg-error "'std::cout' was policy-removed" }
  std::cerr << "b\n";		// { dg-error "'std::cerr' was policy-removed" }
  std::clog << "c\n";
  std::regex r ("x");		// { dg-error "basic_regex<char>' was policy-removed" }
  std::wregex w (L"x");		// { dg-error "basic_regex<wchar_t>' was policy-removed" }
  std::clog << std::endl;
}

void
g ()
{
  [[policy::suppress(cerr)]];
  std::cerr << "b\n";
  std::cout << "a\n";		// { dg-error "'std::cout' was policy-removed" }
}
