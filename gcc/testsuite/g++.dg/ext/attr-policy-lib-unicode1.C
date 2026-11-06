// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
// Policy unicode removes std::cout, std::cerr, std::clog, std::basic_regex,
// fopen, printf and puts, but not std::endl, and can be activated before the
// headers that remove them are included.
[[policy::activate(unicode)]];

#include <iostream>
#include <regex>
#include <cstdio>

void
f ()
{
  std::cout << "a\n";		// { dg-error "'std::cout' was policy-removed" }
  std::cerr << "b\n";		// { dg-error "'std::cerr' was policy-removed" }
  std::clog << "c\n";		// { dg-error "'std::clog' was policy-removed" }
  std::regex r ("x");		// { dg-error "basic_regex<char>' was policy-removed" }
  std::wregex w (L"x");		// { dg-error "basic_regex<wchar_t>' was policy-removed" }
  std::clog << std::endl;	// { dg-error "'std::clog' was policy-removed" }
  std::fopen ("a", "r");	// { dg-error "'FILE\\* fopen\\(const char\\*, const char\\*\\)' was policy-removed" }
  std::printf ("x");		// { dg-error "'int printf\\(const char\\*, ...\\)' was policy-removed" }
  std::puts ("x");		// { dg-error "'int puts\\(const char\\*\\)' was policy-removed" }
  std::fflush (stdout);
}

void
g ()
{
  [[policy::suppress(cerr)]];
  std::cerr << "b\n";
  std::cout << "a\n";		// { dg-error "'std::cout' was policy-removed" }
}
