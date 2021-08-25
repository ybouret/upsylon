
//! \file

#ifndef Y_MKL_TAO_V1_INCLUDED
#define Y_MKL_TAO_V1_INCLUDED


//==============================================================================
//
// LD
//
//==============================================================================

//! load same value in all lhs
template <typename LHS> static inline
void ld(LHS &lhs, typename LHS::param_type value)
{
    for(size_t i=lhs.size();i>0;--i)
    {
        lhs[i] = value;
    }
}

//! load at most n same value in lhs
template <typename LHS> static inline
void ld(LHS &lhs, typename LHS::param_type value, const size_t n)
{
    for(size_t i=min_of<size_t>(n,lhs.size());i>0;--i)
    {
        lhs[i] = value;
    }
}

//==============================================================================
//
// SET
//
//==============================================================================

//! set lhs from rhs, lhs.size() <= rhs.size()
template <typename LHS, typename RHS> static inline
void set(LHS &lhs, RHS &rhs)
{
    assert(lhs.size()<=rhs.size());
    for(size_t i=lhs.size();i>0;--i)
    {
        lhs[i] = Y_TAO_CAST(LHS,RHS,rhs[i]);
    }
}

//! set lhs from rhs, lhs.size() >= rhs.size()
template <typename LHS, typename RHS> static inline
void upload(LHS &lhs, RHS &rhs)
{
    assert(lhs.size()>=rhs.size());
    for(size_t i=rhs.size();i>0;--i)
    {
        lhs[i] = Y_TAO_CAST(LHS,RHS,rhs[i]);
    }
}

//==============================================================================
//
// ADD
//
//==============================================================================

//! target = lhs + rhs, based on target.size()
template <typename TARGET, typename LHS, typename RHS> static inline
void add(TARGET &target, LHS &lhs, RHS &rhs)
{
    assert(target.size()<=lhs.size());
    assert(target.size()<=rhs.size());
    for(size_t i=target.size();i>0;--i)
    {
        target[i] = Y_TAO_CAST(TARGET,LHS,lhs[i]) + Y_TAO_CAST(TARGET,RHS,rhs[i]);
    }
}

//! target += rhs, based and target.size()
template <typename TARGET, typename RHS> static inline
void add(TARGET &target, RHS &rhs)
{
    assert(target.size()<=rhs.size());
    for(size_t i=target.size();i>0;--i)
    {
        target[i] += Y_TAO_CAST(TARGET,RHS,rhs[i]);
    }
}

//==============================================================================
//
// NEG
//
//==============================================================================

//! target=-rhs, based on target.size()
template <typename TARGET, typename RHS> static inline
void neg(TARGET &target, RHS &rhs)
{
    assert(target.size()<=rhs.size());
    for(size_t i=target.size();i>0;--i)
    {
        target[i] = -Y_TAO_CAST(TARGET,RHS,rhs[i]);
    }
}

//! target = -target
template <typename TARGET> static inline
void neg(TARGET &target)
{
    for(size_t i=target.size();i>0;--i)
    {
        target[i] = -target[i];
    }
}


//==============================================================================
//
// SUB
//
//==============================================================================
//! target = lhs-rhs, based on target.size()
template <typename TARGET, typename LHS, typename RHS> static inline
void sub(TARGET &target, LHS &lhs, RHS &rhs)
{
    assert(target.size()<=lhs.size());
    assert(target.size()<=rhs.size());
    for(size_t i=target.size();i>0;--i)
    {
        target[i] = Y_TAO_CAST(TARGET,LHS,lhs[i]) - Y_TAO_CAST(TARGET,RHS,rhs[i]);
    }
}

//! target -= rhs, based on target.size()
template <typename TARGET, typename RHS> static inline
void sub(TARGET &target, RHS &rhs)
{
    assert(target.size()<=rhs.size());
    for(size_t i=target.size();i>0;--i)
    {
        target[i] -= Y_TAO_CAST(TARGET,RHS,rhs[i]);
    }
}

//! target = rhs - target, based on target.size()
template <typename TARGET, typename RHS> static inline
void subp(TARGET &target, RHS &rhs)
{
    assert(target.size()<=rhs.size());
    for(size_t i=target.size();i>0;--i)
    {
        target[i] = Y_TAO_CAST(TARGET,RHS,rhs[i])-target[i];
    }
}

//==============================================================================
//
// MUL
//
//==============================================================================

//! target = value * lhs
template <typename TARGET, typename LHS> static inline
void mulset(TARGET &target, typename TARGET::param_type value, LHS &lhs )
{
    assert(target.size()<=lhs.size());
    for(size_t i=target.size();i>0;--i)
    {
        target[i] = value * Y_TAO_CAST(TARGET,LHS,lhs[i]);
    }
}

//! target *= value
template <typename TARGET> static inline
void mulset(TARGET &target, typename TARGET::param_type value)
{
    for(size_t i=target.size();i>0;--i)
    {
        target[i] *= value;
    }
}

//==============================================================================
//
// DIV
//
//==============================================================================

//! target = (1/value)* lhs
template <typename TARGET, typename LHS> static inline
void divset(TARGET &target, typename TARGET::param_type value, LHS &lhs )
{
    assert(target.size()<=lhs.size());
    for(size_t i=target.size();i>0;--i)
    {
        target[i] =  Y_TAO_CAST(TARGET,LHS,lhs[i]) / value;
    }
}

//! target /= value
template <typename TARGET> static inline
void divset(TARGET &target, typename TARGET::param_type value)
{
    for(size_t i=target.size();i>0;--i)
    {
        target[i] /= value;
    }
}

//! target ./= lhs
template <typename TARGET, typename LHS> static inline
void divall(TARGET &target, LHS &lhs )
{
    assert(target.size()<=lhs.size());
    for(size_t i=target.size();i>0;--i)
    {
        target[i] /=  Y_TAO_CAST(TARGET,LHS,lhs[i]);
    }
}

//==============================================================================
//
// MULADD
//
//==============================================================================

//! target = lhs + value * lhs, based on target.size()
template <typename TARGET, typename LHS, typename RHS> static inline
void muladd(TARGET &target, LHS &lhs, typename TARGET::param_type value, RHS &rhs )
{
    assert(target.size()<=lhs.size());
    assert(target.size()<=rhs.size());
    for(size_t i=target.size();i>0;--i)
    {
        target[i] = Y_TAO_CAST(TARGET,LHS,lhs[i]) + value * Y_TAO_CAST(TARGET,RHS,rhs[i]);
    }
}

//! target += value * lhs, based on target.size()
template <typename TARGET, typename LHS> static inline
void muladd(TARGET &target, typename TARGET::param_type value, LHS &lhs )
{
    assert(target.size()<=lhs.size());
    for(size_t i=target.size();i>0;--i)
    {
        target[i] += value * Y_TAO_CAST(TARGET,LHS,lhs[i]);
    }
}

//==============================================================================
//
// RANGE
//
//==============================================================================

//! range of source
template <typename T, typename U, typename SOURCE> static inline
void range(T &lower, U &upper, SOURCE &source)
{
    lower    = 0;
    upper    = 0;
    size_t n = source.size();
    if(n>0)
    {
        typename SOURCE::mutable_type lo = source[n], up = source[n];
        for(--n;n>0;--n)
        {
            const typename SOURCE::const_type &tmp = source[n];
            if(tmp<lo)
            {
                lo = tmp;
            }
            else if(up<tmp)
            {
                up = tmp;
            }
        }
        lower = auto_cast<T,typename SOURCE::type>::_(lo);
        upper = auto_cast<U,typename SOURCE::type>::_(up);
    }
}

//==============================================================================
//
// swap
//
//==============================================================================

//! swap based on lhs.size()
template <typename LHS, typename RHS> static inline
void swap(LHS &lhs, RHS &rhs)
{
    assert(rhs.size()>=lhs.size());
    for(size_t i=lhs.size();i>0;--i)
    {
        cswap(lhs[i],rhs[i]);
    }

}
#endif
