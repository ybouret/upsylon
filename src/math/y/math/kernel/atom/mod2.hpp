
//! sequential mod2
template <typename ARRAY> static inline
typename real_for< typename ARRAY::mutable_type >::type mod2( ARRAY &arr )
{
    typedef typename real_for< typename ARRAY::mutable_type >::type real_type;
    real_type  sum = 0;
    for(size_t i=arr.size();i>0;--i)
    {
        sum += __mod2( arr[i] );
    }
    return sum;
}

//! sequential mod2
template <typename LHS, typename RHS> static inline
typename real_for< typename LHS::mutable_type >::type mod2( LHS &lhs, RHS &rhs )
{
    assert(rhs.size()>=lhs.size());
    typedef typename LHS::mutable_type        lhs_type;
    typedef typename real_for<lhs_type>::type real_type;
    real_type sum = 0;
    for(size_t i=lhs.size();i>0;--i)
    {
        const lhs_type delta = lhs[i] - Y_MK_ATOM_CAST(LHS,RHS,rhs[i]);
        sum += __mod2( delta );
    }
    return sum;
}



