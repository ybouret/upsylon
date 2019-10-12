//! \file

template <typename LHS,typename RHS> static inline
typename LHS::mutable_type dot( const LHS &lhs, const RHS &rhs )
{
    assert( lhs.size() <= rhs.size() );
    typename LHS::mutable_type ans(0);
    
    for(size_t i=lhs.size();i>0;--i)
    {
        ans += lhs[i] * rhs[i];
    }
    
    return ans;
}

#define Y_MK_ATOM_DOT(i) sum += lhs[i] * rhs[i]

template <typename LHS,typename RHS> static inline
typename LHS::mutable_type dot( const LHS &lhs, const RHS &rhs, concurrent::for_each &loop )
{
    assert( lhs.size() <= rhs.size() );
    typedef typename LHS::mutable_type type;
    concurrent::executor &par = loop.engine();
    par.acquire_for<type>();
    
    Y_MK_ATOM_OPS_API()
    const LHS *lhs_;
    const RHS *rhs_;
    Y_MK_ATOM_OPS_GET(self);
    const LHS &lhs = *self.lhs_;
    const RHS &rhs = *self.rhs_;
    Y_MK_ATOM_OPS_USE_WITH(type &sum = ctx.get<type>();sum=0,
                           lhs.size(),DOT)
    &lhs, &rhs
    Y_MK_ATOM_OPS_RUN(loop);
    
    return par.sum<type>();
}

#undef Y_MK_ATOM_DOT

