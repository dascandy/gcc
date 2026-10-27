// { dg-do compile { target c++17 } }

#if !__has_cpp_attribute(subset::remove)
#error "subset::remove not supported"
#endif

// Accepted.
[[subset::remove(core)]] void f1 ();
[[subset::remove(core)]] void f2 () {}
[[subset::remove(core, "use g instead")]] void f4 ();
[[subset::remove(core, "use " "g")]] void f5 ();
template <typename T> [[subset::remove(core)]] void f3 (T) {}
struct [[subset::remove(core)]] S1 {
  [[subset::remove(core)]] void m1 ();
  [[subset::remove(core)]] void m2 () {}
  [[subset::remove(core)]] S1 ();
  template <typename T> [[subset::remove(core)]] void m3 (T) {}
  [[subset::remove(core)]] static inline int i1 = 0;
  static int i2;
};
[[subset::remove(core)]] void S1::m1 () {}
[[subset::remove(core)]] int S1::i2 = 0;
union [[subset::remove(core)]] U1 { int i; };
template <typename T> struct [[subset::remove(core)]] S2 {};
template <typename T> struct [[subset::remove(core)]] S2<T *> {};
[[subset::remove(core)]] int v1;
[[subset::remove(core)]] extern int v2 = 1;	// { dg-warning "initialized and declared" }
template <typename T> [[subset::remove(core)]] T v3 = T ();
[[using subset: remove(core)]] int v4;

void
g ()
{
  [[subset::remove(core)]] int l1;
  [[subset::remove(core)]] static int l2;
}

// Rejected.
[[subset::remove(core)]] extern int e1;		// { dg-warning "'subset::remove' attribute only applies to" }
struct S3 {
  [[subset::remove(core)]] static int s;		// { dg-warning "only applies to" }
  [[subset::remove(core)]] int d;			// { dg-warning "only applies to" }
};
struct [[subset::remove(core)]] F;			// { dg-warning "only applies to" }
void p ([[subset::remove(core)]] int);		// { dg-warning "only applies to" }
[[subset::remove(core)]] typedef int T1;		// { dg-warning "only applies to" }
using T2 [[subset::remove(core)]] = int;		// { dg-warning "only applies to" }
enum [[subset::remove(core)]] E { e };		// { dg-warning "only applies to" }
int [[subset::remove(core)]] t;			// { dg-warning "attribute ignored" }

[[subset::remove]] void a1 ();			// { dg-error "wrong number of arguments" }
[[subset::remove()]] void a2 ();		// { dg-error "parentheses must be omitted|wrong number of arguments" }
[[subset::remove(1)]] void a3 ();		// { dg-error "'subset::remove' attribute argument must be a subset group name" }
[[subset::remove(core, 1)]] void a4 ();	// { dg-error "reason must be a string literal" }
[[subset::remove(core, "a", "b")]] void a5 ();	// { dg-error "wrong number of arguments" }
