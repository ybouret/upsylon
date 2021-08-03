

//! \file

#ifndef Y_MKL_TAO_V3_INCLUDED
#define Y_MKL_TAO_V3_INCLUDED

template <typename T> static inline
void ldm(matrix<T> &M, typename type_traits<T>::parameter_type value )
{
    M.ld(value);
}


//==============================================================================
//
// MMUL matrix*matrix
//
//==============================================================================

//! M = A*B
template <typename T,typename U,typename V> static inline
void mmul( matrix<T> &M, const matrix<U> &A, const matrix<V> &B)
{
    assert(A.rows==M.rows);
    assert(B.cols==M.cols);
    assert(A.cols==B.rows);

    const size_t nc = M.cols;
    const size_t n  = A.cols;
    for(size_t i=M.rows;i>0;--i)
    {
        array<T>       &M_i = M[i];
        const array<U> &A_i = A[i];
        for(size_t j=nc;j>0;--j)
        {
            T result = 0;
            for(size_t k=n;k>0;--k)
            {
                result += auto_cast<T,U>::_(A_i[k]) * auto_cast<T,V>::_(B[k][j]);
            }
            M_i[j] = result;
        }
    }
}

//! M = A*B'
template <typename T,typename U,typename V> static inline
void mmul_rtrn( matrix<T> &M, const matrix<U> &A, const matrix<V> &B)
{
    assert(M.rows==A.rows);
    assert(M.cols==B.rows);
    assert(A.cols==B.cols);
    
    const size_t nc = M.cols;
    for(size_t i=M.rows;i>0;--i)
    {
        array<T>       &M_i = M[i];
        const array<U> &A_i = A[i];
        for(size_t j=nc;j>0;--j)
        {
            M_i[j] = dot<T>:: of(A_i,B[j]);
        }
    }
}

//! M = A*B'
template <typename T,typename U,typename V> static inline
void mmul_rtrn( matrix<T> &M, const matrix<U> &A, const matrix<V> &B, matrix<T> &N)
{
    assert(M.rows==A.rows);
    assert(M.cols==B.rows);
    assert(A.cols==B.cols);
    assert(N.rows==M.rows);
    assert(N.cols==M.cols);

    const size_t nc = M.cols;
    for(size_t i=M.rows;i>0;--i)
    {
        array<T>       &M_i = M[i];
        array<T>       &N_i = N[i];
        const array<U> &A_i = A[i];
        for(size_t j=nc;j>0;--j)
        {
            N_i[j] = (M_i[j] = dot<T>:: of(A_i,B[j]));
        }
    }
}

//! M = A*A'
template <typename T, typename U> static inline
void gram( matrix<T> &M, const matrix<U> &A )
{
    assert(M.rows==A.rows);
    assert(M.cols==A.rows);

    // half
    const size_t n = M.rows;
    for(size_t i=n;i>0;--i)
    {
        array<T>       &M_i = M[i];
        const array<U> &A_i = A[i];
        for(size_t j=n;j>=i;--j)
        {
            M_i[j] = dot<T>:: of(A_i,A[j]);
        }
    }

    for(size_t i=n;i>0;--i)
    {
        for(size_t j=i-1;j>0;--j)
        {
            M[i][j] = M[j][i];
        }
    }

}

//! M = A'*B
template <typename T,typename U,typename V> static inline
void mmul_ltrn( matrix<T> &M, const matrix<U> &A, const matrix<V> &B)
{
    assert(M.rows==A.cols);
    assert(M.cols==B.cols);
    assert(A.rows==B.rows);
    
    const size_t nc = M.cols;
    const size_t n  = A.rows;
    for(size_t i=M.rows;i>0;--i)
    {
        array<T> &M_i = M[i];
        for(size_t j=nc;j>0;--j)
        {
            T result = 0;
            for(size_t k=n;k>0;--k)
            {
                result += A[k][i] * B[k][j];
            }
            M_i[j] = result;
        }
    }
    
}

//==============================================================================
//
//! getting mod2 for matrices
//
//==============================================================================
template <typename T>
struct mmod2
{
    typedef typename real_for<T>::type real_type; //!< matching real type

    //! for M[r,:]
    static inline
    real_type row_of(const matrix<T> &M, const size_t r)
    {
        return mod2<T>:: of( M[r] );
    }

    //! for M[:,c]
    static inline
    real_type col_of(const matrix<T> &M, const size_t c)
    {
        real_type res = 0;
        for(size_t i=M.rows;i>0;--i)
        {
            res += mod2_of(M[i][c]);
        }
        return res;
    }

    //! row_of(M,r)+col_of(M.c)
    static inline
    real_type both_of(const matrix<T> &M, const size_t r, const size_t c)
    {
        return row_of(M,r) + col_of(M,c);
    }

    //! sum of all |terms|^2
    static inline
    real_type of(const matrix<T> &M)
    {
        real_type res = 0;
        for(size_t i=M.rows;i>0;--i)
        {
            res += mod2<T>:: of( M[i] );
        }
        return res;
    }

    //! sum of all |terms|^2
    template <typename U> static inline
    real_type of(const matrix<T> &A, const matrix<U> &B)
    {
        assert(A.same_size_than(B));
        real_type    res = 0;
        for(size_t i=A.rows;i>0;--i)
        {
            res += mod2<T>::of(A[i],B[i]);
        }
        return res;
    }

};



#endif

