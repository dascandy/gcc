// A caller-side check of a constructor or destructor of a class with a
// virtual base: the wrapper of the base-object variant, which a derived
// class's constructor and destructor call, has its VTT parameter after
// `this', and the contracts' parameters map to the ones after it.  The
// precondition's parameter used to be mapped onto the VTT parameter (ICE
// in verify_gimple), and a postcondition hit an assert in remap_contract.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-client-check=pre -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

static int npre, npost;
static bool P (bool b) { ++npre; return b; }
static bool Q (bool b) { ++npost; return b; }

struct VB { int b = 1; };

struct D : virtual VB
{
  int m;
  D (const int x) pre (P (x > 0)) post (Q (m == x));
  ~D () pre (P (m > 0)) post (Q (true));
};

D::D (const int x) : m (x) {}
D::~D () {}

struct E : D { E () : D (2) {} };

int
main ()
{
  {
    E e;
    if (e.m != 2)
      __builtin_abort ();
  }
  // Each precondition is checked by the caller and by the callee, each
  // postcondition by the callee.
  if (npre != 4 || npost != 2)
    __builtin_abort ();
}
