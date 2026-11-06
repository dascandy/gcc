// A system header for policy-label-run.C: a `policy (group)' term applies
// in a system header too, so that a user who activates the group has the
// library's assertions labelled with it enforced.
#pragma GCC system_header

inline int sys_f (int x) pre <policy(grp_a)> (x > 0) { return x; }
