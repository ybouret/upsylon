

//! \file

#ifndef Y_MKL_TAO_V3_INCLUDED
#define Y_MKL_TAO_V3_INCLUDED

//! M = A*B
template <typename T,typename U,typename V> static inline
void mmul( matrix<T> &M, const matrix<U> &A, const matrix<V> &B)
{
    assert(A.rows==M.rows);
    assert(B.cols==M.cols);
    assert(A.cols==B.rows);

    const size_t c = M.cols;
    const size_t n = A.cols;
    for(size_t i=M.rows;i>0;--i)
    {
        array<T>       &M_i = M[i];
        const array<U> &A_i = A[i];
        for(size_t j=c;j>0;--j)
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
void mmul_trn( matrix<T> &M, const matrix<U> &A, const matrix<V> &B)
{
    assert(M.rows==A.rows);
    assert(M.cols==B.rows);
    assert(A.cols==B.cols);
    
    const size_t c = M.cols;
    for(size_t i=M.rows;i>0;--i)
    {
        array<T>       &M_i = M[i];
        const array<U> &A_i = A[i];
        for(size_t j=c;j>0;--j)
        {
            M_i[j] = dot<T>:: of(A_i,B[j]);
        }

    }

}




#endif

