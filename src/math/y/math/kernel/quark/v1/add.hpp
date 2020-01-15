//! \file

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

//! mul add operation with casts
#define Y_QUARK_muladd(I) tgt[I] = Y_QUARK_CAST(TARGET,LHS,lhs[I]) + x * Y_QUARK_CAST(TARGET,RHS,rhs[I])
Y_QUARK_IMPL3(muladd)
#undef Y_QUARK_muladd

template <typename TARGET, typename SOURCE> static inline
void muladd( TARGET &target, typename TARGET::param_type x, SOURCE &source )
{
    muladd(target,target,x,source);
}

template <typename TARGET, typename SOURCE> static inline
void muladd( TARGET &target, typename TARGET::param_type x, SOURCE &source, concurrent::for_each &loop )
{
    muladd(target,target,x,source,loop);
}


