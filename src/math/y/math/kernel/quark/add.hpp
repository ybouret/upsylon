
//! tgt=lhs+rhs
#define Y_QUARK_add(I) tgt[I] = Y_QUARK_CAST(TARGET,LHS,lhs[I]) + Y_QUARK_CAST(TARGET,RHS,rhs[I])
Y_QUARK_IMPL2(add)
#undef Y_QUARK_add

//! tgt+=source
template <typename TARGET,typename SOURCE> static inline
void add( TARGET &target, SOURCE &source )
{
    add(target,target,source);
}

//! tgt+=source
template <typename TARGET,typename SOURCE> static inline
void add( TARGET &target, SOURCE &source, concurrent::for_each &loop )
{
    add(target,target,source,loop);
}
