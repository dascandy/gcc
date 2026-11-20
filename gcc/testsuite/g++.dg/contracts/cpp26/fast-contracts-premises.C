// -ffast-contracts: at the postcheck entry point the preconditions are
// assumed by evaluating them, so the optimizers can use what they say
// about memory and the results of calls -- including the call of a
// function whose predicate makes one, which is not assumed at all
// without -ffast-contracts.
// { dg-do compile { target { c++26 && { i?86-*-linux* x86_64-*-linux* } } } }
// { dg-additional-options "-fcontracts -ffast-contracts -O2" }

void dead ();
[[gnu::const]] bool ok (int);

struct S
{
  int v;
  int size () const { return v; }
  int get () const pre (v > 0);
};

int S::get () const { if (v <= 0) dead (); return v; }
int by_ref (const S &s) pre (s.v > 0) { if (s.v <= 0) dead (); return s.v; }
int by_call (int x) pre (ok (x)) { if (!ok (x)) dead (); return x; }
int by_inline (const S &s) pre (s.size () > 0) { if (s.v <= 0) dead (); return s.v; }

// Each second entry point holds the body, without the call of dead.
// { dg-final { scan-assembler-not {\.postcheck:\n(?:(?!\t\.size)[^\n]*\n)*?\t(?:call|jmp)\t_Z4deadv} } }
// { dg-final { scan-assembler-times {\n_ZNK1S3getEv\.postcheck:\n} 1 } }
// { dg-final { scan-assembler-times {\n_Z6by_refRK1S\.postcheck:\n} 1 } }
// { dg-final { scan-assembler-times {\n_Z7by_calli\.postcheck:\n} 1 } }
// { dg-final { scan-assembler-times {\n_Z9by_inlineRK1S\.postcheck:\n} 1 } }
