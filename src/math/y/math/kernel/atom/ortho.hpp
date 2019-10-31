
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
    typename TARGET::mutable_type num=0,den=0;
    for(size_t i=n;i>0;--i)
    {
        typename TARGET::const_type l = Y_MK_ATOM_CAST(TARGET,LHS,lhs[i]);
        typename TARGET::const_type r = Y_MK_ATOM_CAST(TARGET,RHS,rhs[i]);
        num += l*r;
        den += r*r;
    }
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
template <typename TARGET,typename LHS, typename RHS> static inline
void ortho(TARGET &target, RHS &rhs )
{
    ortho(target,target,rhs);
}
