
//! sequential rms
template <typename ARRAY> static inline
typename real_for< typename ARRAY::mutable_type >::type rms( ARRAY &arr )
{
    typedef typename real_for< typename ARRAY::mutable_type >::type real_type;
    real_type       sum = 0;
    const    size_t n   = arr.size();

    if(n>0)
    {
        for(size_t i=n;i>0;--i)
        {
            sum += __mod2( arr[i] );
        }
        return sqrt_of( sum/n );
    }
    else
    {
        return sum;
    }
}


//! sequential rms
template <typename LHS, typename RHS> static inline
typename real_for< typename LHS::mutable_type >::type rms( LHS &lhs, RHS &rhs )
{
    assert(rhs.size()>=lhs.size());
    typedef typename LHS::mutable_type        lhs_type;
    typedef typename real_for<lhs_type>::type real_type;
    real_type    sum = 0;
    const size_t n   = lhs.size();
    if(n>0)
    {
        for(size_t i=lhs.size();i>0;--i)
        {
            const lhs_type delta = lhs[i] - Y_MK_ATOM_CAST(LHS,RHS,rhs[i]);
            sum += __mod2( delta );
        }
        return sqrt_of( sum/n );
    }
    else
    {
        return sum;
    }
}
