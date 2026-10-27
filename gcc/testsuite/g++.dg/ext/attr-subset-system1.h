#pragma GCC system_header
// Code in a system header, parsed and instantiated where every subset is
// active, that would break each of their rules in user code.

#include <cstdarg>

int sys_printf (const char *, ...);

struct sys_comma { sys_comma operator, (int) { return *this; } };
struct sys_addr { sys_addr *operator& () { return this; } };
struct sys_and { bool operator&& (int) { return true; } };

inline int
sys_all (int *p, bool a, bool b, const int *cp, sys_comma c, sys_addr s)
{
  ;
  p = p + 1;
  ++p;
  (void) p[1];
  a = a & b;
  a |= b;
  sys_printf ("%d", 1);
  c, 1;
  (void) &s;
  (void) reinterpret_cast<long> (p);
  (void) const_cast<int *> (cp);
  int *n = new int;
  delete n;
  sys_removed ();
  if (a)
    throw 1;
  return 0;
}

template <typename T>
T *
sys_template (T *p, bool a)
{
  ;
  p = p + 1;
  a = a | a;
  sys_printf ("%d", 1);
  T *n = new T;
  delete n;
  sys_removed ();
  (void) a;
  throw T ();
}
