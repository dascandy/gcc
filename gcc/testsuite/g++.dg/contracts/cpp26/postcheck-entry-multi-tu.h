// Shared by postcheck-entry-multi-tu.C and postcheck-entry-multi-tu-2.cc.

extern int npre;
bool P (bool);

int defined_elsewhere (const int x) pre (P (x > 0)) post (r: r == x + 1);

struct T
{
  int t;
  T (const int x) pre (P (x > 0)) post (t == x);
};

inline int in_both (int x) pre (P (x > 0)) { return x; }

int other_tu (int);
