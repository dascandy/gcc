// { dg-do compile { target c++11 } }
// { dg-additional-options "-D_GLIBCXX_SYSHDR" }
// The predefined groupings are known without any header; their members
// are not.
[[subset::activate(cout)]];		// { dg-warning "unknown subset 'cout'" }
[[subset::activate(unicode)]];
[[subset::activate(c_string)]];
[[subset::activate(inefficient)]];
