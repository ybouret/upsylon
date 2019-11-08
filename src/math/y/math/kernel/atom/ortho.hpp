
//! target = lhs - (lhs.rhs)rhs/(rhs.rhs)
/**
 remove component of lhs on rhs, target is orthogonal to rhs
 */
template <typename TARGET,typename LHS, typename RHS> static inline
void ortho(TARGET &target, LHS &lhs, RHS &rhs )
{
    assert( target.size() == lhs.size() );
    assert( target.size() == rhs.size() );
    const    size_t               n = target.size();
    // first pass: accumulate numerator and denominator
    typename TARGET::mutable_type num=0,den=0;
    for(size_t i=n;i>0;--i)
    {
        typename TARGET::const_type l = Y_MK_ATOM_CAST(TARGET,LHS,lhs[i]);
        typename TARGET::const_type r = Y_MK_ATOM_CAST(TARGET,RHS,rhs[i]);
        num += l*r;
        den += r*r;
    }

    // second pass: removing component
    if(den>0)
    {
        for(size_t i=n;i>0;--i)
        {
            target[i] = Y_MK_ATOM_CAST(TARGET,LHS,lhs[i]) - ( num * Y_MK_ATOM_CAST(TARGET,RHS,rhs[i]) ) / den;
        }
    }
    else
    {
        set(target,lhs);
    }
}

//! make target orthogonal to rhs
template <typename TARGET,typename RHS> static inline
void ortho(TARGET &target, RHS &rhs )
{
    ortho(target,target,rhs);
}

//! target = lhs - (lhs.rhs)rhs/(rhs.rhs)
/**
 remove component of lhs on rhs, target is orthogonal to rhs
 */
template <typename TARGET,typename LHS, typename RHS, typename TMP> static inline
void ortho_safe(TARGET &target, LHS &lhs, RHS &rhs, TMP &tmp)
{
    assert( target.size() == lhs.size() );
    assert( target.size() == rhs.size() );
    const    size_t               n = target.size();
    for(size_t i=n;i>0;--i)
    {
        typename TARGET::const_type r = Y_MK_ATOM_CAST(TARGET,RHS,rhs[i]);
        tmp[i] = r*r;
    }
    typename TARGET::const_type den = sorted_sum(tmp);

    if(den>0)
    {
        for(size_t i=n;i>0;--i)
        {
            typename TARGET::const_type l = Y_MK_ATOM_CAST(TARGET,LHS,lhs[i]);
            typename TARGET::const_type r = Y_MK_ATOM_CAST(TARGET,RHS,rhs[i]);
            tmp[i] = l*r;
        }
        typename TARGET::const_type num = sorted_sum_by_abs(tmp);
        for(size_t i=n;i>0;--i)
        {
            target[i] = Y_MK_ATOM_CAST(TARGET,LHS,lhs[i]) - ( num * Y_MK_ATOM_CAST(TARGET,RHS,rhs[i]) ) / den;
        }
    }
    else
    {
        set(target,lhs);
    }
}
