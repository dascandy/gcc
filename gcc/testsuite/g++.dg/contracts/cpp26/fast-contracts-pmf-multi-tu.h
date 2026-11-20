// Shared by fast-contracts-pmf-multi-tu.C and fast-contracts-pmf-multi-tu-2.cc.

struct B { virtual int f (int x) pre (x > 0); };
inline int B::f (int x) { return x; }

int (B::*other_tu ()) (int);
