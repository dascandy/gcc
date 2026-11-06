// { dg-do compile { target c++17 } }

#if !__has_cpp_attribute(policy::remove)
#error "policy::remove not supported"
#endif

// Accepted.
[[policy::remove(core)]] void f1 ();
[[policy::remove(core)]] void f2 () {}
[[policy::remove(core, "use g instead")]] void f4 ();
[[policy::remove(core, "use " "g")]] void f5 ();
template <typename T> [[policy::remove(core)]] void f3 (T) {}
struct [[policy::remove(core)]] S1 {
  [[policy::remove(core)]] void m1 ();
  [[policy::remove(core)]] void m2 () {}
  [[policy::remove(core)]] S1 ();
  template <typename T> [[policy::remove(core)]] void m3 (T) {}
  [[policy::remove(core)]] static inline int i1 = 0;
  static int i2;
};
[[policy::remove(core)]] void S1::m1 () {}
[[policy::remove(core)]] int S1::i2 = 0;
union [[policy::remove(core)]] U1 { int i; };
template <typename T> struct [[policy::remove(core)]] S2 {};
template <typename T> struct [[policy::remove(core)]] S2<T *> {};
[[policy::remove(core)]] int v1;
[[policy::remove(core)]] extern int v2 = 1;	// { dg-warning "initialized and declared" }
[[policy::remove(core)]] extern int v5;
template <typename T> [[policy::remove(core)]] T v3 = T ();
[[using policy: remove(core)]] int v4;
[[policy::remove(core)]] typedef int T1;
using T2 [[policy::remove(core)]] = int;
template <typename T> using T3 [[policy::remove(core)]] = T;

void
g ()
{
  [[policy::remove(core)]] int l1;
  [[policy::remove(core)]] static int l2;
}

// Rejected.
struct S3 {
  [[policy::remove(core)]] static int s;		// { dg-warning "only applies to" }
  [[policy::remove(core)]] int d;			// { dg-warning "only applies to" }
};
struct [[policy::remove(core)]] F;			// { dg-warning "only applies to" }
void p ([[policy::remove(core)]] int);		// { dg-warning "only applies to" }
enum [[policy::remove(core)]] E { e };		// { dg-warning "only applies to" }
void
h ()
{
  [[policy::remove(core)]] extern int e1;		// { dg-warning "only applies to" }
}
int [[policy::remove(core)]] t;			// { dg-warning "attribute ignored" }

[[policy::remove]] void a1 ();			// { dg-error "wrong number of arguments" }
[[policy::remove()]] void a2 ();		// { dg-error "parentheses must be omitted|wrong number of arguments" }
[[policy::remove(1)]] void a3 ();		// { dg-error "'policy::remove' attribute argument must be a policy group name" }
[[policy::remove(core, 1)]] void a4 ();	// { dg-error "reason must be a string literal" }
[[policy::remove(core, "a", "b")]] void a5 ();	// { dg-error "wrong number of arguments" }
