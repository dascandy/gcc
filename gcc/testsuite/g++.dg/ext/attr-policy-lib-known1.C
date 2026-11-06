// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
// The predefined groupings are known without any header; their members
// are not.
[[policy::activate(cout)]];		// { dg-warning "unknown policy 'cout'" }
[[policy::activate(unicode)]];
[[policy::activate(c_string)]];
[[policy::activate(wide_string)]];
[[policy::activate(inefficient)]];
