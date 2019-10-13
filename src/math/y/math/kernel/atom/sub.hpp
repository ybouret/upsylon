//! \file

//! SIMD kernel
#define Y_MK_ATOM_SUB(I) target[I] -= static_cast<typename TARGET::const_type>(source[I])


//! sequential target -= source
template <typename TARGET, typename SOURCE> static inline
void sub( TARGET &target, const SOURCE &source )
{
    assert( target.size() <= source.size() );
    for(size_t i=target.size();i>0;--i)
    {
        Y_MK_ATOM_SUB(i);
    }
}


//! parallel target -= source
template <typename TARGET, typename SOURCE> static inline
void sub( TARGET &target, const SOURCE &source,  concurrent::for_each &loop )
{
    assert( target.size() <= source.size() );

    Y_MK_ATOM_OPS_API()
    TARGET       *target_;
    const SOURCE *source_;
    Y_MK_ATOM_OPS_GET(self);
    TARGET       &target = *self.target_;
    const SOURCE &source = *self.source_;
    Y_MK_ATOM_OPS_USE(target.size(),SUB)
    &target, &source
    Y_MK_ATOM_OPS_RUN(loop);
    
}

#undef Y_MK_ATOM_SUB


//! SIMD kernel
#define Y_MK_ATOM_SUB(offset) \
target[offset] = static_cast<typename TARGET::const_type>(lhs[offset]) - static_cast<typename TARGET::const_type>(rhs[offset])

//! sequential target = lhs-rhs
template <typename TARGET, typename LHS, typename RHS> static inline
void sub( TARGET &target, const LHS &lhs, const RHS &rhs )
{
    assert( target.size() <= lhs.size() );
    assert( target.size() <= rhs.size() );
    for(size_t i=target.size();i>0;--i)
    {
        Y_MK_ATOM_SUB(i);
    }
}


//! parallel target = lhs-rhs
template <typename TARGET, typename LHS, typename RHS> static inline
void sub( TARGET &target, const LHS &lhs, const RHS &rhs, concurrent::for_each &loop )
{
    assert( target.size() <= lhs.size() );
    assert( target.size() <= rhs.size() );

    Y_MK_ATOM_OPS_API()
    TARGET       *target_;
    const LHS    *lhs_;
    const RHS    *rhs_;
    Y_MK_ATOM_OPS_GET(self);
    TARGET       &target = *self.target_;
    const LHS    &lhs    = *self.lhs_;
    const RHS    &rhs    = *self.rhs_;
    Y_MK_ATOM_OPS_USE(target.size(),SUB)
    &target, &lhs, &rhs
    Y_MK_ATOM_OPS_RUN(loop);
    
}

#undef Y_MK_ATOM_SUB

//! SIMD kernel
#define Y_MK_ATOM_SUBP(I) target[I] = static_cast<typename TARGET::const_type>(source[I])-target[I]

//! sequential target = source-target
template <typename TARGET, typename SOURCE> static inline
void subp( TARGET &target, const SOURCE &source )
{
    assert( target.size() <= source.size() );
    for(size_t i=target.size();i>0;--i)
    {
        Y_MK_ATOM_SUBP(i);
    }
}


//! parallel target = source-target
template <typename TARGET, typename SOURCE> static inline
void subp( TARGET &target, const SOURCE &source,  concurrent::for_each &loop )
{
    assert( target.size() <= source.size() );
    
    Y_MK_ATOM_OPS_API()
    TARGET       *target_;
    const SOURCE *source_;
    Y_MK_ATOM_OPS_GET(self);
    TARGET       &target = *self.target_;
    const SOURCE &source = *self.source_;
    Y_MK_ATOM_OPS_USE(target.size(),SUBP)
    &target, &source
    Y_MK_ATOM_OPS_RUN(loop);
    
}

#undef Y_MK_ATOM_SUBP
