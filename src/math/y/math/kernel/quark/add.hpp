
//! tgt=lhs+rhs
#define Y_QUARK_add(I) tgt[I] = Y_QUARK_CAST(TARGET,LHS,lhs[I]) + Y_QUARK_CAST(TARGET,RHS,rhs[I])
Y_QUARK_IMPL2(add)
#undef Y_QUARK_add
