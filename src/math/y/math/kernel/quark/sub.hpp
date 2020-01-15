


#define Y_QUARK_sub(I) tgt[I] = Y_QUARK_CAST(TARGET,LHS,lhs[I]) - Y_QUARK_CAST(TARGET,RHS,rhs[I])
//! tgt=lhs-rhs
Y_QUARK_IMPL2(sub)
#undef Y_QUARK_sub

template <typename TARGET,typename SOURCE> static inline
void sub( TARGET &target, SOURCE &source )
{
    sub( target, target, source );
}

template <typename TARGET,typename SOURCE> static inline
void sub( TARGET &target, SOURCE &source, concurrent::for_each &loop )
{
    sub( target, target, source, loop);
}


#define Y_QUARK_subp(I) tgt[I] = Y_QUARK_CAST(TARGET,RHS,rhs[I]) - Y_QUARK_CAST(TARGET,LHS,lhs[I])
//! tgt=rhs-lhs
Y_QUARK_IMPL2(subp)
#undef Y_QUARK_subp

template <typename TARGET,typename SOURCE> static inline
void subp( TARGET &target, SOURCE &source )
{
    subp( target, target, source );
}

template <typename TARGET,typename SOURCE> static inline
void subp( TARGET &target, SOURCE &source, concurrent::for_each &loop )
{
    subp( target, target, source, loop);
}
