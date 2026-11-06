#pragma GCC system_header
// C-style casts in a system header, parsed and instantiated where policies
// reinterpret_cast and const_cast are active.

inline long sys_r (int *p) { return (long) p; }
inline int *sys_c (const int *p) { return (int *) p; }
template<typename T> long sys_tr (T *p) { return (long) p; }
template<typename T> T *sys_tc (const T *p) { return (T *) p; }
