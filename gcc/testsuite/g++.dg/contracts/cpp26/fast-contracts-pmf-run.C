// -ffast-contracts with P3097: pointers to member functions that point to
// the thunk of a virtual function dispatch to the final overrider through
// every conversion: to a derived class, where the function's class is a
// base other than the first, and back; in a variable's initializer; and
// with GCC's conversion of a pointer to member function to a pointer to
// function, which yields the thunk.  Equal pointers compare equal.
// { dg-do run { target c++26 } }
// The conversion to a pointer to function is an extension, so no -pedantic.
// { dg-options "-fcontracts -fcontracts-p3097 -ffast-contracts -O2 -Wno-pmf-conversions" }

struct A { int a = 1; virtual int h (int x) { return x; } };
struct B { int b = 2; virtual int f (int x) pre (x > 0) { return x; } };
struct D : A, B { int f (int x) override { return x + 10; } };

typedef int (B::*BF) (int);
typedef int (D::*DF) (int);

BF global_bf = &B::f;
DF global_df = &B::f;

__attribute__ ((noipa)) int callb (B *b, BF p, int x) { return (b->*p) (x); }
__attribute__ ((noipa)) int calld (D *d, DF p, int x) { return (d->*p) (x); }
__attribute__ ((noipa)) BF opaque (BF p) { return p; }

int
main ()
{
  D d;
  B b;
  if (callb (&b, &B::f, 1) != 1 || callb (&d, &B::f, 1) != 11)
    __builtin_abort ();
  DF df = &B::f;
  if (calld (&d, df, 2) != 12 || calld (&d, global_df, 3) != 13)
    __builtin_abort ();
  BF back = static_cast<BF> (df);
  if (callb (&d, back, 4) != 14 || callb (&d, global_bf, 5) != 15)
    __builtin_abort ();
  if (opaque (&B::f) != &B::f || back != &B::f || df != global_df)
    __builtin_abort ();
  typedef int (*FP) (B *, int);
  FP fp = (FP) &B::f;
  if (fp (static_cast<B *> (&d), 6) != 16)
    __builtin_abort ();
}
