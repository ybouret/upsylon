
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


#endif
