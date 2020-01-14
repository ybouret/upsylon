


//! tgt=lhs-rhs
#define Y_QUARK_sub(I) tgt[I] = Y_QUARK_CAST(TARGET,LHS,lhs[I]) - Y_QUARK_CAST(TARGET,RHS,rhs[I])
Y_QUARK_IMPL2(sub)
#undef Y_QUARK_sub


//! tgt=rhs-lhs
#define Y_QUARK_subp(I) tgt[I] = Y_QUARK_CAST(TARGET,RHS,rhs[I]) = Y_QUARK_CAST(TARGET,LHS,lhs[I])
Y_QUARK_IMPL2(subp)
#undef Y_QUARK_subp
