// -fcontract-postcheck-entry: postcondition captures (P3098) are
// initialized at the postcheck entry point, after every precondition.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098 -fcontract-postcheck-entry -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

static int order[8], n;
static bool P (bool b) { order[n++] = 1; return b; }
static int CAP (int v) { order[n++] = 2; return v; }

int f (int x)
  post [a = CAP (x)] (r: r == a + 1)
  pre (P (x > 0))
  pre (P (x < 100))
{ return x + 1; }

int main ()
{
  if (f (1) != 2)
    __builtin_abort ();
  /* Both preconditions, then the capture, although the capture comes
     first in the declaration.  */
  if (n != 3 || order[0] != 1 || order[1] != 1 || order[2] != 2)
    __builtin_abort ();
}
