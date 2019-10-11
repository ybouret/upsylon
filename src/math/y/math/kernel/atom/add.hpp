template <typename TARGET, typename SOURCE> static inline
void add( TARGET &target, const SOURCE &source )
{
    assert( target.size() <= source.size() );
    for(size_t i=target.size();i>0;--i) target[i] += static_cast<typename TARGET::const_type>(source[i]);
}


#define Y_MK_ATOM_ADD(I) target[I] += static_cast<typename TARGET::const_type>(source[I])

template <typename TARGET, typename SOURCE> static inline
void add( TARGET &target, const SOURCE &source,  concurrent::for_each &loop )
{
    assert( target.size() <= source.size() );

    Y_MK_ATOM_OPS_API()
    TARGET       *target_;
    const SOURCE *source_;
    Y_MK_ATOM_OPS_GET(self);
    TARGET       &target = *self.target_;
    const SOURCE &source = *self.source_;
    Y_MK_ATOM_OPS_USE(target.size(),ADD)
    &target, &source
    Y_MK_ATOM_OPS_RUN(loop);
    
}

#undef Y_MK_ATOM_ADD

#define Y_MK_ATOM_ADD(offset) \
target[offset] = static_cast<typename TARGET::const_type>(lhs[offset]) + static_cast<typename TARGET::const_type>(rhs[offset])


template <typename TARGET, typename LHS, typename RHS> static inline
void add( TARGET &target, const LHS &lhs, const RHS &rhs )
{
    assert( target.size() <= lhs.size() );
    assert( target.size() <= rhs.size() );
    for(size_t i=target.size();i>0;--i)
    {
        target[i] = static_cast<typename TARGET::const_type>(lhs[i]) + static_cast<typename TARGET::const_type>(rhs[i]);
    }
}


template <typename TARGET, typename LHS, typename RHS> static inline
void add( TARGET &target, const LHS &lhs, const RHS &rhs, concurrent::for_each &loop )
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
    Y_MK_ATOM_OPS_USE(target.size(),ADD)
    &target, &lhs, &rhs
    Y_MK_ATOM_OPS_RUN(loop);
}

#undef Y_MK_ATOM_ADD

