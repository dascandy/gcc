// { dg-do compile { target c++17 } }
// Subset raw_new_delete disallows new and delete expressions, and calls to
// operator new, operator delete, operator new[] and operator delete[].
// Templates that use them, like std::vector and std::shared_ptr, can still
// be used, as they are checked where they are defined.

#include <new>
#include <memory>
#include <vector>
#include <string>

struct B { virtual ~B (); };
struct C
{
  static void *operator new (std::size_t);
  static void operator delete (void *);
};

[[subset::activate(raw_new_delete)]];

using A4 = int[4];

void
use (int n, void *buf)
{
  int *p = new int;				// { dg-error "'new' expression in subset 'raw_new_delete'" }
  int *a = new int[n];				// { dg-error "'new\\\[\\\]' expression in subset 'raw_new_delete'" }
  int *a4 = new A4;				// { dg-error "'new\\\[\\\]' expression" }
  int *g = ::new int (1);			// { dg-error "'new' expression" }
  int *pl = new (buf) int (2);			// { dg-error "'new' expression" }
  C *c = new C;					// { dg-error "'new' expression" }
  delete p;					// { dg-error "'delete' expression in subset 'raw_new_delete'" }
  delete[] a;					// { dg-error "'delete\\\[\\\]' expression in subset 'raw_new_delete'" }
  ::delete[] a4;				// { dg-error "'delete\\\[\\\]' expression" }
  delete c;					// { dg-error "'delete' expression" }
  (void) pl; (void) g;

  void *r = ::operator new (16);		// { dg-error "call to 'operator new' in subset 'raw_new_delete'" }
  ::operator delete (r);			// { dg-error "call to 'operator delete' in subset 'raw_new_delete'" }
  r = operator new[] (16);			// { dg-error "call to 'operator new \\\[\\\]'" }
  operator delete[] (r);			// { dg-error "call to 'operator delete \\\[\\\]'" }
  r = C::operator new (sizeof (C));		// { dg-error "call to 'operator new'" }
  C::operator delete (r);			// { dg-error "call to 'operator delete'" }

  // Allowed.
  auto u = std::make_unique<int> (3);
  auto s = std::make_shared<int> (4);
  std::vector<int> v (4);
  v.push_back (5);
  std::string str (100, 'x');
  std::shared_ptr<B> sb (std::make_shared<B> ());
  std::allocator<int> al;
  int *x = al.allocate (2);
  al.deallocate (x, 2);
  B b;
}

// A template is checked where it is defined, even if never instantiated.
template <typename T>
T *
tf ()
{
  return new T;					// { dg-error "'new' expression" }
}

[[subset::suppress(raw_new_delete)]];

void
again ()
{
  delete new int;
}
