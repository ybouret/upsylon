//! \file

//! tgt=lhs+rhs
#define Y_QUARK_add(I) tgt[I] = Y_QUARK_CAST(TARGET,LHS,lhs[I]) + Y_QUARK_CAST(TARGET,RHS,rhs[I])
Y_QUARK_IMPL2(add)
#undef Y_QUARK_add

//! target+=source
template <typename TARGET,typename SOURCE> static inline
void add(TARGET &target, SOURCE &source)
{
    add(target,target,source);
}

//! target+=source
template <typename TARGET,typename SOURCE> static inline
void add(TARGET &target, SOURCE &source, concurrent::for_each &loop)
{
    add(target,target,source,loop);
}

//! muladd operation with casts
#define Y_QUARK_muladd(I) tgt[I] = Y_QUARK_CAST(TARGET,LHS,lhs[I]) + x * Y_QUARK_CAST(TARGET,RHS,rhs[I])
Y_QUARK_IMPL3(muladd)
#undef Y_QUARK_muladd

//! target += x*source
template <typename TARGET, typename SOURCE> static inline
void muladd(TARGET &target, typename TARGET::param_type x, SOURCE &source)
{
    std::cerr << "quark.muladd.seq x=" << x << ":" << binary<typename TARGET::mutable_type>(x) << std::endl;
    muladd(target,target,x,source);
}

//! target += x*source, parallel
template <typename TARGET, typename SOURCE> static inline
void muladd(TARGET &target, typename TARGET::param_type x, SOURCE &source, concurrent::for_each &loop)
{
    std::cerr << "quark.muladd.par x=" << x << ":" << binary<typename TARGET::mutable_type>(x) << std::endl;
    muladd(target,target,x,source,loop);
}


