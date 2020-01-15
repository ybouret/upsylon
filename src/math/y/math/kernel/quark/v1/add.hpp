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
#define Y_QUARK_mul_add(I) tgt[I] = Y_QUARK_CAST(TARGET,LHS,lhs[I]) + x * Y_QUARK_CAST(TARGET,RHS,rhs[I])
Y_QUARK_IMPL3(mul_add)
#undef Y_QUARK_mul_add

template <typename TARGET, typename SOURCE> static inline
void mul_add( TARGET &target, typename TARGET::param_type x, SOURCE &source )
{
    mul_add(target,target,x,source);
}

template <typename TARGET, typename SOURCE> static inline
void mul_add( TARGET &target, typename TARGET::param_type x, SOURCE &source, concurrent::for_each &loop )
{
    mul_add(target,target,x,source,loop);
}


