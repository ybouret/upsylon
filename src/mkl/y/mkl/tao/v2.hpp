
//! \file

#ifndef Y_MKL_TAO_V2_INCLUDED
#define Y_MKL_TAO_V2_INCLUDED

//==============================================================================
//
//! getting dot products
//
//==============================================================================
template <typename T>
struct dot
{
    //! based on lhs.size()
    template <typename LHS, typename RHS> static inline
    T of(LHS &lhs, RHS &rhs)
    {
        assert(lhs.size()<=rhs.size());
        T result = 0;
        for(size_t i=lhs.size();i>0;--i)
        {
            result += Y_TAO_TO(T,LHS,lhs[i]) * Y_TAO_TO(T,RHS,rhs[i]);
        }
        return result;
    }
};

//==============================================================================
//
//! getting mod2
//
//==============================================================================
template <typename T>
struct mod2
{
    typedef typename real_for<T>::type real_type; //!< matching real type

    //! single vector
    template <typename LHS> static inline
    real_type of(LHS &lhs)
    {
        real_type result = 0;
        for(size_t i=lhs.size();i>0;--i)
        {
            result += mod2_of( Y_TAO_TO(T,LHS,lhs[i]) );
        }
        return result;
    }

    //! based on lhs.size()
    template <typename LHS, typename RHS> static inline
    real_type of(LHS &lhs, RHS &rhs)
    {
        assert(lhs.size()<=rhs.size());
        real_type result = 0;
        for(size_t i=lhs.size();i>0;--i)
        {
            result += mod2_of( Y_TAO_TO(T,LHS,lhs[i]) - Y_TAO_TO(T,RHS,rhs[i]));
        }
        return result;
    }
};

//==============================================================================
//
//! getting rms
//
//==============================================================================
template <typename T>
struct rms
{
    typedef typename real_for<T>::type real_type; //!< matching real type

    //! single vector
    template <typename LHS>
    real_type of(LHS &lhs)
    {
        real_type    res = 0;
        const size_t dof = lhs.size();
        for(size_t i=dof;i>0;--i)
        {
            res += mod2_of( Y_TAO_TO(T,LHS,lhs[i]) );
        }
        return sqrt_of(res/dof);
    }

    //! based on lhs.size()
    template <typename LHS, typename RHS> static inline
    T of(LHS &lhs, RHS &rhs)
    {
        assert(lhs.size()<=rhs.size());
        real_type    res = 0;
        const size_t dof = lhs.size();
        for(size_t i=dof;i>0;--i)
        {
            res  += mod2_of( Y_TAO_TO(T,LHS,lhs[i]) - Y_TAO_TO(T,RHS,rhs[i]));
        }
        return sqrt_of(res/dof);
    }
};


//==============================================================================
//
//! diagonal operations
//
//==============================================================================
struct diag
{
    //! get diagonal items
    template <typename TARGET, typename T> static inline
    void get(TARGET &diag, const matrix<T> &M)
    {
        assert(M.rows<=diag.size());assert(M.cols>=M.rows);
        for(size_t i=M.rows;i>0;--i)
        {
            diag[i] =  auto_cast<typename TARGET::type,T>::_(M[i][i]);
        }
    }
};



//==============================================================================
//
// MUL matrix*vector
//
//==============================================================================


//! target = M * rhs, based on target.size() <= M.rows, M.cols <= rhs.size()
template <typename TARGET, typename T, typename LHS> static inline
void mul(TARGET &target, const matrix<T> &M, LHS &rhs)
{
    assert(target.size()<=M.rows);
    assert(M.cols <= rhs.size());
    for(size_t j=target.size();j>0;--j)
    {
        target[j] = dot<typename TARGET::mutable_type>::of(M[j],rhs);
    }
}

//! target = -M * rhs, based on target.size() <= M.rows, M.cols <= rhs.size()
template <typename TARGET, typename T, typename LHS> static inline
void mul_neg(TARGET &target, const matrix<T> &M, LHS &rhs)
{
    assert(target.size()<=M.rows);
    assert(M.cols <= rhs.size());
    for(size_t j=target.size();j>0;--j)
    {
        target[j] = -dot<typename TARGET::mutable_type>::of(M[j],rhs);
    }
}


//! target -= M * rhs, based on target.size() <= M.rows, M.cols <= rhs.size()
template <typename TARGET, typename T, typename LHS> static inline
void mul_sub(TARGET &target, const matrix<T> &M, LHS &rhs)
{
    assert(target.size()<=M.rows);
    assert(M.cols <= rhs.size());
    for(size_t j=target.size();j>0;--j)
    {
        target[j] -= dot<typename TARGET::mutable_type>::of(M[j],rhs);
    }
}

//! target += M * rhs, based on target.size() <= M.rows, M.cols <= rhs.size()
template <typename TARGET, typename T, typename LHS> static inline
void mul_add(TARGET &target, const matrix<T> &M, LHS &rhs)
{
    assert(target.size()<=M.rows);
    assert(M.cols <= rhs.size());
    for(size_t j=target.size();j>0;--j)
    {
        target[j] += dot<typename TARGET::mutable_type>::of(M[j],rhs);
    }
}


//! [SLOW] target = M' * rhs, based on target.size() <= M.cols, M.rows <= rhs.size()
template <typename TARGET, typename T, typename RHS> static inline
void mul_trn(TARGET &target, const matrix<T> &M, RHS &rhs)
{
    assert(target.size()<= M.cols);
    assert(M.rows       <= rhs.size());
    typedef typename TARGET::mutable_type type;
    const size_t r = M.rows;
    for(size_t j=target.size();j>0;--j)
    {
        type result = 0;
        for(size_t k=r;k>0;--k)
        {
            result += auto_cast<type,T>::_(M[k][j]) * Y_TAO_CAST(TARGET,RHS,rhs[k]);
        }
        target[j] = result;
    }
}


#endif
