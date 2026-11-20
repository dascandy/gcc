// Second translation unit of fast-contracts-pmf-multi-tu.C.

#include "fast-contracts-pmf-multi-tu.h"

int (B::*other_tu ()) (int) { return &B::f; }
