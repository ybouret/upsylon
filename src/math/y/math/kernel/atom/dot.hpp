//! \file

#define Y_MK_ATOM_DOT(i) sum += lhs[i] * rhs[i]

template <typename LHS,typename RHS> static inline
typename LHS::mutable_type dot( const LHS &lhs, const RHS &rhs )
{
    assert( lhs.size() <= rhs.size() );
    typename LHS::mutable_type sum(0);
    
    for(size_t i=lhs.size();i>0;--i)
    {
       // ans += lhs[i] * rhs[i];
        Y_MK_ATOM_DOT(i);
    }
    
    return sum;
}


template <typename LHS,typename RHS> static inline
typename LHS::mutable_type dot( const LHS &lhs, const RHS &rhs, concurrent::for_each &loop )
{
    assert( lhs.size() <= rhs.size() );
    concurrent::executor &par = loop.engine();
    typedef typename LHS::mutable_type type;

    {
        const type __zero(0);
        par.build_from<type>(__zero);
    }

    Y_MK_ATOM_OPS_API()
    const LHS *lhs_;
    const RHS *rhs_;
    Y_MK_ATOM_OPS_GET(self);
    const LHS &lhs = *self.lhs_;
    const RHS &rhs = *self.rhs_;
    Y_MK_ATOM_OPS_USE_WITH(type &sum = ctx.get<type>(),
                           lhs.size(),DOT)
    &lhs, &rhs
    Y_MK_ATOM_OPS_RUN(loop);
    
    return par.sum<type>();
}

#undef Y_MK_ATOM_DOT


#define Y_MK_ATOM_NORM2(i) do { typename ARRAY::const_type &a = arr[i]; sum += a*a; } while(false)

template <typename ARRAY> static inline
typename ARRAY::mutable_type norm2( const ARRAY &arr )
{
    typename ARRAY::mutable_type sum(0);

    for(size_t i=arr.size();i>0;--i)
    {
        Y_MK_ATOM_NORM2(i);
    }

    return sum;
}

template <typename ARRAY> static inline
typename ARRAY::mutable_type norm2( const ARRAY &arr,  concurrent::for_each &loop )
{
    concurrent::executor &par = loop.engine();
    typedef typename ARRAY::mutable_type type;

    {
        const type __zero(0);
        par.build_from<type>(__zero);
    }

    Y_MK_ATOM_OPS_API()
    const ARRAY *arr_;
    Y_MK_ATOM_OPS_GET(self);
    const ARRAY &arr = *self.arr_;
    Y_MK_ATOM_OPS_USE_WITH(type &sum = ctx.get<type>(),
                           arr.size(),NORM2)
    &arr
    Y_MK_ATOM_OPS_RUN(loop);

    return par.sum<type>();
}
